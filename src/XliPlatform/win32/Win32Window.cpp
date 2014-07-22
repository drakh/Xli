#include <XliPlatform/PlatformSpecific/Win32Window.h>
#include <XliPlatform/PlatformSpecific/Win32Helpers.h>
#include <XliPlatform/Display.h>
#include <Xli/Console.h>
#include <Xli/HashMap.h>
#include <Xli/Unicode.h>
#include <cstdlib>

// Windows XP look and feel
#pragma comment(linker, \
    "\"/manifestdependency:type='Win32' " \
    "name='Microsoft.Windows.Common-Controls' " \
    "version='6.0.0.0' " \
    "processorArchitecture='*' " \
    "publicKeyToken='6595b64144ccf1df' " \
    "language='*'\"")

// From windowsx.h
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

namespace Xli
{
    static int InitCount = 0;
    static HINSTANCE HInstance = 0;
    static LPCWSTR WindowClassName = L"XliWindow";
    static PlatformSpecific::Win32Window* MainWindow = 0;
    static SystemCursor CurrentCursor = SystemCursorArrow;

    namespace PlatformSpecific
    {
        static DWORD StyleFromXliWindowFlags(int flags)
        {
            DWORD dwStyle;

            if (flags & WindowFlagsBorderless)
                dwStyle = WS_POPUP;
            else if (flags & WindowFlagsResizeable)
                dwStyle = WS_OVERLAPPEDWINDOW;
            else // WindowFlagsFixed
                dwStyle = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

            return dwStyle;
        }

        Win32Window::Win32Window(int width, int height, const Xli::String& title, int flags)
        {
            this->ownsHwnd = true;
            this->closed = false;
            this->fullscreen = false;
            this->eventHandler = eventHandler;

            if (flags & WindowFlagsFullscreen)
            {
                flags &= ~WindowFlagsFullscreen;
                fullscreen = true;
            }

            rect.top = 0;
            rect.left = 0;
            rect.bottom = height;
            rect.right = width;

            dwStyle = StyleFromXliWindowFlags(flags);
            AdjustWindowRect(&rect, dwStyle, 0);

            Utf16String titleW = Unicode::Utf8To16(title);
            hWnd = CreateWindowW(WindowClassName, titleW.DataPtr(), dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, 0, 0, HInstance, 0);

            if (!hWnd)
                XLI_THROW("Failed to create window: " + Win32Helpers::GetLastErrorString());

            if (!MainWindow)
                MainWindow = this;

            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

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

            dwStyle = GetWindowLong(hWnd, GWL_STYLE);
            GetWindowRect(hWnd, &rect);
        }

        Win32Window::~Win32Window()
        {
            if (this == MainWindow)
                MainWindow = 0;

            if (ownsHwnd)
                DestroyWindow(hWnd);
        }

        WindowImplementation Win32Window::GetImplementation()
        {
            return WindowImplementationWin32;
        }

        void Win32Window::SetEventHandler(WindowEventHandler* handler)
        {
            eventHandler = handler;
        }

        WindowEventHandler* Win32Window::GetEventHandler()
        {
            return eventHandler;
        }

        void Win32Window::Close()
        {
            bool cancel = false;

            if (!eventHandler || !eventHandler->OnClosing(this, cancel) || !cancel)
            {
                CloseWindow(hWnd);
                closed = true;
            }
        }

        void Win32Window::RegisterTouchEvents()
        {
            RegisterTouchWindow(hWnd, 0);
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

        int Win32Window::GetDisplayIndex()
        {
            RECT rect;
            GetWindowRect(hWnd, &rect);

            Vector2i p(
                rect.left + (rect.right - rect.left) / 2, 
                rect.top + (rect.bottom - rect.top) / 2);

            for (int i = 0; i < Display::GetCount(); i++)
                if (Display::GetRect(i).Intersects(p.X, p.Y))
                    return i;

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

        void Win32Window::SetTitle(const String& title)
        {
            Utf16String titleW = Unicode::Utf8To16(title);
            SetWindowTextW(hWnd, titleW.DataPtr());
        }

        String Win32Window::GetTitle()
        {
            int l = GetWindowTextLengthW(hWnd);
            if (l == 0) return String();

            Utf16String titleW = Utf16String::Create(l);
            l = GetWindowText(hWnd, titleW.DataPtr(), l);

            return Unicode::Utf16To8(titleW);
        }

        void Win32Window::SetFullscreen(bool fullscreen)
        {
            if (this->fullscreen != fullscreen)
            {
                this->fullscreen = fullscreen;

                if (fullscreen)
                {
                    GetWindowRect(hWnd, &rect);

                    Recti display = Display::GetRect(GetDisplayIndex());

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

        SystemCursor Win32Window::GetSystemCursor()
        {
            return CurrentCursor;
        }

        void Win32Window::SetSystemCursor(SystemCursor cursor)
        {
            CurrentCursor = cursor;
            ::ShowCursor(cursor != SystemCursorNone);
        }

        static Key VirtualToXliKey(WPARAM vk)
        {
            // http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

            switch (vk)
            {
            case VK_TAB:        return KeyTab;
            case VK_RETURN:        return KeyEnter;
            case VK_SPACE:        return KeySpace;
            case VK_LMENU:        return KeyAlt;
            case VK_RMENU:        return KeyAlt;
            case VK_SHIFT:        return KeyShift;
            case VK_CONTROL:    return KeyCtrl;
            case VK_CAPITAL:    return KeyCapsLock;
            case VK_UP:            return KeyUp;
            case VK_DOWN:        return KeyDown;
            case VK_LEFT:        return KeyLeft;
            case VK_RIGHT:        return KeyRight;
            case VK_BACK:        return KeyBackspace;
            case VK_F1:            return KeyF1;
            case VK_F2:            return KeyF2;
            case VK_F3:            return KeyF3;
            case VK_F4:            return KeyF4;
            case VK_F5:            return KeyF5;
            case VK_F6:            return KeyF6;
            case VK_F7:            return KeyF7;
            case VK_F8:            return KeyF8;
            case VK_F9:            return KeyF9;
            case VK_F10:        return KeyF10;
            case VK_F11:        return KeyF11;
            case VK_F12:        return KeyF12;
            case VK_PRIOR:        return KeyPageUp;
            case VK_NEXT:        return KeyPageDown;
            case VK_HOME:        return KeyHome;
            case VK_END:        return KeyEnd;
            case VK_INSERT:        return KeyInsert;
            case VK_DELETE:        return KeyDelete;
            case VK_ESCAPE:        return KeyEscape;
            }

            return KeyUnknown;
        }

        static int VirtualFromXliKey(Key key)
        {
            switch (key)
            {
            case KeyTab:        return VK_TAB;
            case KeyEnter:        return VK_RETURN;
            case KeySpace:        return VK_SPACE;
            case KeyAlt:        return VK_MENU;
            case KeyShift:        return VK_SHIFT;
            case KeyCtrl:        return VK_CONTROL;
            case KeyCapsLock:    return VK_CAPITAL;
            case KeyUp:            return VK_UP;
            case KeyDown:        return VK_DOWN;
            case KeyLeft:        return VK_LEFT;
            case KeyRight:        return VK_RIGHT;
            case KeyBackspace:    return VK_BACK;
            case KeyF1:            return VK_F1;
            case KeyF2:            return VK_F2;
            case KeyF3:            return VK_F3;
            case KeyF4:            return VK_F4;
            case KeyF5:            return VK_F5;
            case KeyF6:            return VK_F6;
            case KeyF7:            return VK_F7;
            case KeyF8:            return VK_F8;
            case KeyF9:            return VK_F9;
            case KeyF10:        return VK_F10;
            case KeyF11:        return VK_F11;
            case KeyF12:        return VK_F12;
            case KeyPageUp:        return VK_PRIOR;
            case KeyPageDown:    return VK_NEXT;
            case KeyHome:        return VK_HOME;
            case KeyEnd:        return VK_END;
            case KeyInsert:        return VK_INSERT;
            case KeyDelete:        return VK_DELETE;
            case KeyEscape:        return VK_ESCAPE;
            }

            return 0;
        }

        static int VirtualFromXliMouseButton(MouseButton buttons)
        {
            switch (buttons)
            {
            case MouseButtonLeft:
                return GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON;

            case MouseButtonMiddle:
                return VK_MBUTTON;

            case MouseButtonRight:
                return GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON;

            case MouseButtonX1:
                return VK_XBUTTON1;

            case MouseButtonX2:
                return VK_XBUTTON2;
            }

            return 0;
        }

        bool Win32Window::GetKeyState(Key key)
        {
            int vk = VirtualFromXliKey(key);
            return vk && (GetAsyncKeyState(vk) & 0x8000);
        }

        bool Win32Window::GetMouseButtonState(MouseButton button)
        {
            int vk = VirtualFromXliMouseButton(button);
            return vk && (GetAsyncKeyState(vk) & 0x8000);
        }

        Vector2i Win32Window::GetMousePosition()
        {
            POINT p;
            ::GetCursorPos(&p);
            ::ScreenToClient(hWnd, &p);
            return Vector2i(p.x, p.y);
        }

        void Win32Window::SetMousePosition(Vector2i position)
        {
            POINT p;
            p.x = position.X;
            p.y = position.Y;
            ::ClientToScreen(hWnd, &p);
            ::SetCursorPos(p.x, p.y);
        }

        void Win32Window::SetIconByID(int id)
        {
            SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id)));
            SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id)));
        }

        LRESULT CALLBACK Win32Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            Win32Window* wnd = reinterpret_cast<Win32Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

            static const unsigned int MOUSEEVENTF_FROMTOUCH = 0xff515700;

            switch (message)
            {
            case WM_KEYDOWN:
                if (wnd->eventHandler)
                {
                    Key key = VirtualToXliKey(wParam);
                    if (key != KeyUnknown && wnd->eventHandler->OnKeyDown(wnd, key))
                        return 0;
                    /*
                    else
                    TranslateMessage(&msg);
                    */
                }
                break;

            case WM_KEYUP:
                if (wnd->eventHandler)
                {
                    Key key = VirtualToXliKey(wParam);
                    if (key != KeyUnknown && wnd->eventHandler->OnKeyUp(wnd, key))
                        return 0;
                }
                break;

            case WM_CHAR:
                if (wnd->eventHandler && wnd->eventHandler->OnTextInput(wnd, (String)(char)(unsigned char)(int)wParam))
                    return 0;

                break;

            case WM_MOUSEMOVE:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseMove(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))))
                    return 0;

                break;
                // http://msdn.microsoft.com/en-us/library/windows/desktop/ms646265(v=vs.85).aspx
                /*
            case WM_MOUSELEAVE:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseMove(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))))
                    return 0;

                break;
                */
            case WM_MBUTTONDOWN:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseDown(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), MouseButtonMiddle))
                    return 0;

                break;

            case WM_MBUTTONUP:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseUp(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), MouseButtonMiddle))
                    return 0;

                break;

            case WM_LBUTTONDOWN:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseDown(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), MouseButtonLeft))
                    return 0;

                break;

            case WM_LBUTTONUP:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseUp(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), MouseButtonLeft))
                    return 0;

                break;

            case WM_RBUTTONDOWN:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseDown(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), MouseButtonRight))
                    return 0;

                break;

            case WM_RBUTTONUP:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseUp(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), MouseButtonRight))
                    return 0;

                break;

            case WM_MOUSEWHEEL:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseWheel(wnd, Vector2i(0, GET_WHEEL_DELTA_WPARAM(wParam))))
                    return 0;

                break;

            case WM_MOUSEHWHEEL:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseWheel(wnd, Vector2i(GET_WHEEL_DELTA_WPARAM(wParam), 0)))
                    return 0;

                break;

            case WM_XBUTTONDOWN:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseDown(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButtonX1 : MouseButtonX2))
                    return 0;

                break;

            case WM_XBUTTONUP:
                if (wnd->eventHandler && wnd->eventHandler->OnMouseUp(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButtonX1 : MouseButtonX2))
                    return 0;

                break;

            case WM_TOUCH:
                if (wnd->eventHandler)
                {
                    int numInputs = LOWORD(wParam);

                    if (numInputs)
                    {
                        PTOUCHINPUT touchPoints = (PTOUCHINPUT)alloca(numInputs * sizeof(TOUCHINPUT));

                        if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs, touchPoints, sizeof(TOUCHINPUT)))
                        {
                            for (int i = 0; i < numInputs; i++)
                            {
                                TOUCHINPUT ti = touchPoints[i];

                                if (ti.dwFlags & TOUCHEVENTF_DOWN)
                                    wnd->eventHandler->OnTouchDown(wnd, Vector2((float)ti.x / 100.0f, (float)ti.y / 100.0f), ti.dwID);

                                if (ti.dwFlags & TOUCHEVENTF_MOVE)
                                    wnd->eventHandler->OnTouchMove(wnd, Vector2((float)ti.x / 100.0f, (float)ti.y / 100.0f), ti.dwID);

                                if (ti.dwFlags & TOUCHEVENTF_UP)
                                    wnd->eventHandler->OnTouchUp(wnd, Vector2((float)ti.x / 100.0f, (float)ti.y / 100.0f), ti.dwID);
                            }

                            CloseTouchInputHandle((HTOUCHINPUT)lParam);
                        }
                        else
                        {
                            ErrorPrintLine("WARNING: Error reading touchpoint info.");
                        }

                        return 0;
                    }
                }
                break;

            case WM_SIZE:
                if (wnd->eventHandler)
                {
                    wnd->eventHandler->OnSizeChanged(wnd, Vector2i(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
                    return 0;
                }

                break;

            case WM_CLOSE:
                if (wnd->eventHandler)
                {
                    bool cancel = false;
                    if (wnd->eventHandler->OnClosing(wnd, cancel) && cancel)
                        return 0;
                }

                DestroyWindow(hWnd);
                break;

            case WM_QUIT:
                wnd->closed = true;
                if (wnd->eventHandler)
                {
                    wnd->eventHandler->OnClosed(wnd);
                    return 0;
                }

                break;

            case WM_DESTROY:
                PostQuitMessage(0);

                wnd->closed = true;
                if (wnd->eventHandler)
                {
                    wnd->eventHandler->OnClosed(wnd);
                    return 0;
                }

                break;
            }

            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    Vector2i Window::GetScreenSize()
    {
        return Vector2i(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }

    void Window::SetMainWindow(Window* wnd)
    {
        MainWindow = wnd->GetImplementation() == WindowImplementationWin32 ? (PlatformSpecific::Win32Window*)wnd : NULL;
    }

    Window* Window::GetMainWindow()
    {
        return MainWindow;
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

    void DisplayInit();
    void DisplayDone();

    void Window::Init()
    {
        if (!InitCount) 
        {
            HInstance = GetModuleHandle(0);

            WNDCLASSEX wcex;
            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style          = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc    = PlatformSpecific::Win32Window::WndProc;
            wcex.cbClsExtra     = 0;
            wcex.cbWndExtra     = 0;
            wcex.hInstance      = HInstance;
            wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
            wcex.hIcon          = LoadIcon(HInstance, IDI_APPLICATION);
            wcex.hIconSm        = LoadIcon(HInstance, IDI_APPLICATION);
            wcex.hbrBackground  = NULL;
            wcex.lpszMenuName   = NULL;
            wcex.lpszClassName  = WindowClassName;

            if (!RegisterClassEx(&wcex)) 
                XLI_THROW("Failed to register window class");

            DisplayInit();
        }

        InitCount++;
    }

    void Window::Done()
    {
        InitCount--;

        if (!InitCount)
        {
            DisplayDone();
            UnregisterClass(WindowClassName, HInstance);
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
            atexit(Window::Done);
        }
    }

    Window* Window::Create(int width, int height, const Xli::String& title, int flags)
    {
        AssertInit();
        return new PlatformSpecific::Win32Window(width, height, title, flags);
    }

    Window* Window::CreateFrom(void* nativeWindowHandle)
    {
        AssertInit();
        return new PlatformSpecific::Win32Window((HWND)nativeWindowHandle);
    }
}