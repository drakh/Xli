#include <XliPlatform/Win32Window.h>
#include <XliPlatform/Win32Helpers.h>
#include <XliPlatform/Display.h>
#include <Xli/HashMap.h>

// Windows XP look and feel.
#pragma comment(linker, \
	"\"/manifestdependency:type='Win32' " \
	"name='Microsoft.Windows.Common-Controls' " \
	"version='6.0.0.0' " \
	"processorArchitecture='*' " \
	"publicKeyToken='6595b64144ccf1df' " \
	"language='*'\"")

namespace Xli
{
	static int InitCount = 0;
	static HINSTANCE hInstance;
	static const LPWSTR windowClassName = L"XliWindow";
	static Win32Window* mainWindow = 0;

	static DWORD XliWindowStyleToWin32Style(int style)
	{
		DWORD dwStyle;
		if ((style & WindowStyleBorderless) == WindowStyleBorderless) dwStyle = WS_POPUP;
		else if ((style & WindowStyleResizeable) == WindowStyleResizeable) dwStyle = WS_OVERLAPPEDWINDOW;
		else dwStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX); // WindowStyleFixed
		return dwStyle;
	}

	Win32Window::Win32Window(int width, int height, const Xli::String& title, WindowEventHandler* eventHandler, int style)
	{
		this->ownsHwnd = true;
		this->closed = false;
		this->fullscreen = false;
		
		this->eventHandler = eventHandler;
		this->emulateMouseAsTouchPoint = false;
		this->capturedMouseTouchPoint = false;

		if ((style & WindowStyleFullscreen) == WindowStyleFullscreen)
		{
			style &= ~WindowStyleFullscreen;
			fullscreen = true;
		}

		rect.top = 0;
		rect.left = 0;
		rect.bottom = height;
		rect.right = width;

		dwStyle = XliWindowStyleToWin32Style(style);
		AdjustWindowRect(&rect, dwStyle, 0);

		hWnd = CreateWindowW(windowClassName, (LPCWSTR)title.Data(), dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, 0, 0, hInstance, 0);

		if (!hWnd)
		{
			XLI_THROW("Failed to create window: " + Win32Helpers::GetLastErrorString());
		}

		if (!mainWindow)
		{
			mainWindow = this;
		}

		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)this);

		ShowWindow(hWnd, SW_NORMAL);
		SetForegroundWindow(hWnd);
		SetFocus(hWnd);

		if (fullscreen)
		{
			fullscreen = false;
			SetFullscreen(true);
		}
	}

	Win32Window::Win32Window(HWND hWnd)
	{
		this->hWnd = hWnd;
		this->closed = false;
		this->ownsHwnd = false;
		this->fullscreen = false;

		this->emulateMouseAsTouchPoint = false;
		this->capturedMouseTouchPoint = false;

		dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		GetWindowRect(hWnd, &rect);
	}
	
	Win32Window::~Win32Window()
	{
		if (this == mainWindow)
		{
			mainWindow = 0;
		}

		if (ownsHwnd)
		{
			DestroyWindow(hWnd);
		}
	}

	Platform Win32Window::GetPlatform()
	{
		return PlatformWin32;
	}

	void Win32Window::Close()
	{
		if (!(eventHandler && !eventHandler->OnClose()))
		{
			CloseWindow(hWnd);
			closed = true;
		}
	}

	void Win32Window::RegisterTouchEvents(bool emulateMouseAsTouchPoint)
	{
		RegisterTouchWindow(hWnd, 0);
		this->emulateMouseAsTouchPoint = emulateMouseAsTouchPoint;
		capturedMouseTouchPoint = false;
	}

	bool Win32Window::IsClosed()
	{
		return closed;
	}

	bool Win32Window::IsVisible()
	{
		return true;
	}

	bool Win32Window::IsFullscreen()
	{
		return fullscreen;
	}

	bool Win32Window::IsMinimized()
	{
		return (GetWindowLong(hWnd, GWL_STYLE) & SW_MINIMIZE) == SW_MINIMIZE;
	}

	bool Win32Window::IsMaximized()
	{
		return (GetWindowLong(hWnd, GWL_STYLE) & SW_MAXIMIZE) == SW_MAXIMIZE;
	}

	int Win32Window::GetDisplay()
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);

		Vector2i p(rect.left + (rect.right - rect.left) / 2, rect.top + (rect.bottom - rect.top) / 2);

		for (int i = 0; i < Display::GetCount(); i++)
		{
			Recti display = Display::GetRect(i);
			if (display.Intersects(p.X, p.Y)) return i;
		}

		return 0;
	}

	Vector2i Win32Window::GetPosition()
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		return Vector2i(rect.left, rect.top);
	}

	Vector2i Win32Window::GetClientSize()
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		return Vector2i(rect.right, rect.bottom);
	}

	Vector2i Window::GetScreenSize()
	{
		return Vector2i(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	}

	Window* Window::GetMainWindow()
	{
		return mainWindow;
	}

	void Win32Window::SetMainWindow()
	{
		mainWindow = this;
	}

	void Win32Window::SetTitle(const String& title)
	{
		SetWindowTextW(hWnd, (LPCWSTR)title.Data());
	}

	String Win32Window::GetTitle()
	{
		int l = GetWindowTextLengthW(hWnd);
		if (l == 0) return String();

		wchar_t* title = new wchar_t[l];
		l = GetWindowText(hWnd, title, l);

		String result(title, l);
		delete [] title;
		return result;
	}

	void Win32Window::SetFullscreen(bool fullscreen)
	{
		if (this->fullscreen != fullscreen)
		{
			this->fullscreen = fullscreen;

			if (fullscreen)
			{
				GetWindowRect(hWnd, &rect);

				Recti display = Display::GetRect(GetDisplay());

				SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
				SetWindowPos(hWnd, 0, display.Left, display.Top, display.Right - display.Left, display.Bottom - display.Top + 1, SWP_NOZORDER | SWP_FRAMECHANGED);
				BringWindowToTop(hWnd);
				ShowWindow(hWnd, SW_SHOWNORMAL);
			}
			else
			{
				SetWindowLong(hWnd, GWL_STYLE, dwStyle);
				SetWindowPos(hWnd, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_FRAMECHANGED);
				ShowWindow(hWnd, SW_SHOWNORMAL);
			}
		}
	}

	void Win32Window::SetPosition(Vector2i pos)
	{
		SetWindowPos(hWnd, 0, pos.X, pos.Y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	void Win32Window::SetClientSize(Vector2i size)
	{
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.right = size.X;
		rect.bottom = size.Y;

		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		AdjustWindowRect(&rect, dwStyle, 0);

		SetWindowPos(hWnd, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);
	}

	void Win32Window::Minimize()
	{
		ShowWindow(hWnd, SW_SHOWMINIMIZED);
	}

	void Win32Window::Maximize()
	{
		ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	}

	void Win32Window::Restore()
	{
		ShowWindow(hWnd, SW_SHOWNORMAL);
	}

	void Win32Window::ShowCursor(bool show)
	{
		::ShowCursor(show);
	}

	void Win32Window::SetIconByID(int id)
	{
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id)));
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id)));
	}

	static Xli::Key translateKey(int vk)
	{
		// http://msdn2.microsoft.com/en-us/library/ms927178.aspx

		switch (vk)
		{
		case VK_TAB:		return KeyTab; break;
		case VK_RETURN:		return KeyEnter; break;
		case VK_SPACE:		return KeySpace; break;
		case VK_LMENU:		return KeyAlt; break;
		case VK_RMENU:		return KeyAlt; break;
		//case VK_MENU:		return KeyAlt; break;
		case VK_SHIFT:		return KeyShift; break;
		case VK_CONTROL:	return KeyCtrl; break;
		case VK_CAPITAL:	return KeyCapsLock; break;
		case VK_UP:			return KeyUp; break;
		case VK_DOWN:		return KeyDown; break;
		case VK_LEFT:		return KeyLeft; break;
		case VK_RIGHT:		return KeyRight; break;
		case VK_BACK:		return KeyBackspace; break;
		case VK_F1:			return KeyF1; break;
		case VK_F2:			return KeyF2; break;
		case VK_F3:			return KeyF3; break;
		case VK_F4:			return KeyF4; break;
		case VK_F5:			return KeyF5; break;
		case VK_F6:			return KeyF6; break;
		case VK_F7:			return KeyF7; break;
		case VK_F8:			return KeyF8; break;
		case VK_F9:			return KeyF9; break;
		case VK_F10:		return KeyF10; break;
		case VK_F11:		return KeyF11; break;
		case VK_F12:		return KeyF12; break;
		/*case VK_F13:		return KeyF13; break;
		case VK_F14:		return KeyF14; break;
		case VK_F15:		return KeyF15; break;
		case VK_F16:		return KeyF16; break;*/
		case VK_PRIOR:		return KeyPageUp; break;
		case VK_NEXT:		return KeyPageDown; break;
		case VK_HOME:		return KeyHome; break;
		case VK_END:		return KeyEnd; break;
		case VK_INSERT:		return KeyInsert; break;
		case VK_DELETE:		return KeyDelete; break;
		case VK_ESCAPE:		return KeyEscape; break;
		default:			return (Xli::Key)vk; break;
		}
	}

	LRESULT CALLBACK Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Win32Window* wnd = reinterpret_cast<Win32Window*>(::GetWindowLong(hWnd, GWL_USERDATA));

		const unsigned int MOUSEEVENTF_FROMTOUCH = 0xff515700;

		switch (message)
		{
		case WM_KEYDOWN: 
			if (wnd->eventHandler) wnd->eventHandler->OnKeyDown(translateKey((int)wParam));
			break;
			
		case WM_KEYUP: 
			if (wnd->eventHandler) wnd->eventHandler->OnKeyUp(translateKey((int)wParam));
			break;

		case WM_CHAR:
			if (wnd->eventHandler) wnd->eventHandler->OnCharTyped((int)wParam);
			break;

		case WM_MOUSEMOVE:
			if (wnd->eventHandler)
			{
				wnd->eventHandler->OnMouseMove(LOWORD(lParam), HIWORD(lParam));
				if (wnd->emulateMouseAsTouchPoint && wnd->capturedMouseTouchPoint)
				{
					wnd->eventHandler->OnTouchMove((float)LOWORD(lParam), (float)HIWORD(lParam), 0);
				}
			}
			break;

		case WM_MOUSELEAVE:
			if (wnd->eventHandler)
			{
				wnd->eventHandler->OnMouseLeave(LOWORD(lParam), HIWORD(lParam));
				if (wnd->emulateMouseAsTouchPoint && wnd->capturedMouseTouchPoint)
				{
					wnd->capturedMouseTouchPoint = false;
					wnd->eventHandler->OnTouchUp((float)LOWORD(lParam), (float)HIWORD(lParam), 0);
				}
			}
			break;

		case WM_MBUTTONDOWN:
			if (wnd->eventHandler) wnd->eventHandler->OnMouseDown(LOWORD(lParam), HIWORD(lParam), MouseButtonsMiddle);		
			break;

		case WM_MBUTTONUP:
			if (wnd->eventHandler) wnd->eventHandler->OnMouseUp(LOWORD(lParam), HIWORD(lParam), MouseButtonsMiddle);
			break;

		case WM_LBUTTONDOWN:
			if (wnd->eventHandler)
			{
				wnd->eventHandler->OnMouseDown(LOWORD(lParam), HIWORD(lParam), MouseButtonsLeft);
				if (wnd->emulateMouseAsTouchPoint && (GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCH) != MOUSEEVENTF_FROMTOUCH)
				{
					wnd->capturedMouseTouchPoint = true;
					wnd->eventHandler->OnTouchDown((float)LOWORD(lParam), (float)HIWORD(lParam), 0);
				}
			}
			break;

		case WM_LBUTTONUP:
			if (wnd->eventHandler)
			{
				wnd->eventHandler->OnMouseUp(LOWORD(lParam), HIWORD(lParam), MouseButtonsLeft);
				if (wnd->emulateMouseAsTouchPoint && (GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCH) != MOUSEEVENTF_FROMTOUCH)
				{
					wnd->capturedMouseTouchPoint = false;
					wnd->eventHandler->OnTouchUp((float)LOWORD(lParam), (float)HIWORD(lParam), 0);
				}
			}
			break;

		case WM_RBUTTONDOWN:
			if (wnd->eventHandler) wnd->eventHandler->OnMouseDown(LOWORD(lParam), HIWORD(lParam), MouseButtonsRight);
			break;

		case WM_RBUTTONUP:
			if (wnd->eventHandler) wnd->eventHandler->OnMouseUp(LOWORD(lParam), HIWORD(lParam), MouseButtonsRight);
			break;

		case WM_MOUSEWHEEL:
			if (wnd->eventHandler) wnd->eventHandler->OnMouseWheel(0, GET_WHEEL_DELTA_WPARAM(wParam));
			break;

		case WM_XBUTTONDOWN:
			if (wnd->eventHandler) wnd->eventHandler->OnMouseDown(LOWORD(lParam), HIWORD(lParam), GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButtonsX1 : MouseButtonsX2);
			break;

		case WM_XBUTTONUP:
			if (wnd->eventHandler) wnd->eventHandler->OnMouseUp(LOWORD(lParam), HIWORD(lParam), GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButtonsX1 : MouseButtonsX2);
			break;

		case WM_TOUCH:
			if (wnd->eventHandler)
			{
				int numInputs = LOWORD(wParam);
				if (numInputs)
				{
					PTOUCHINPUT touchPoints = new TOUCHINPUT[numInputs];
					if (GetTouchInputInfo((HTOUCHINPUT) lParam, numInputs, touchPoints, sizeof(TOUCHINPUT)))
					{
						for (int i = 0; i < numInputs; i++)
						{
							TOUCHINPUT ti = touchPoints[i];
							if (ti.dwFlags & TOUCHEVENTF_DOWN)
								wnd->eventHandler->OnTouchDown((float) ti.x / 100.0f, (float) ti.y / 100.0f, ti.dwID);
							if (ti.dwFlags & TOUCHEVENTF_MOVE)
								wnd->eventHandler->OnTouchMove((float) ti.x / 100.0f, (float) ti.y / 100.0f, ti.dwID);
							if (ti.dwFlags & TOUCHEVENTF_UP)
								wnd->eventHandler->OnTouchUp((float) ti.x / 100.0f, (float) ti.y / 100.0f, ti.dwID);
						}
						CloseTouchInputHandle((HTOUCHINPUT) lParam);
					}
					else
					{
						ErrorPrintLine("WARNING: Error reading touchpoint info.");
					}
					delete [] touchPoints;
				}
			}
			break;

		case WM_SIZE:
			if (wnd->eventHandler) wnd->eventHandler->OnResize(LOWORD(lParam), HIWORD(lParam));			
			break;

		case WM_CLOSE:
			if (!(wnd->eventHandler && !wnd->eventHandler->OnClose())) DestroyWindow(hWnd);
			break;

		case WM_QUIT:
			wnd->closed = true;
			if (wnd->eventHandler) wnd->eventHandler->OnClosed();
			break;

		case WM_DESTROY:
			wnd->closed = true;
			if (wnd->eventHandler) wnd->eventHandler->OnClosed();
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	void Window::ProcessMessages()
	{
		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::Init()
	{
		if (!InitCount) 
		{
			hInstance = GetModuleHandle(0);

			WNDCLASSEX wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style          = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc    = Win32Window::WndProc;
			wcex.cbClsExtra     = 0;
			wcex.cbWndExtra     = 0;
			wcex.hInstance      = hInstance;
			wcex.hIcon          = LoadIcon(hInstance, IDI_APPLICATION);
			wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground  = (HBRUSH)(COLOR_BACKGROUND);
			wcex.lpszMenuName   = NULL;
			wcex.lpszClassName  = windowClassName;
			wcex.hIconSm        = LoadIcon(hInstance, IDI_APPLICATION);

			if (!RegisterClassEx(&wcex)) XLI_THROW("Failed to register window class");
		}

		InitCount++;
	}

	void Window::Shutdown()
	{
		InitCount--;

		if (InitCount == 0)
		{
			UnregisterClass(windowClassName, hInstance);
		}
		else if (InitCount < 0)
		{
			XLI_THROW_BAD_DELETE;
		}
	}

	static void AssertInit()
	{
		if (!InitCount)
		{
			Window::Init();
			atexit(Window::Shutdown);
		}
	}

	Window* Window::Create(int width, int height, const Xli::String& title, WindowEventHandler* eventHandler, int style)
	{
		AssertInit();
		return new Win32Window(width, height, title, eventHandler, style);
	}

	Window* Window::Adopt(void* nativeWindowHandle)
	{
		AssertInit();
		return new Win32Window((HWND)nativeWindowHandle);
	}
}