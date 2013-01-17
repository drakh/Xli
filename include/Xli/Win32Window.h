#ifndef __XLI_WIN32_WINDOW_H__
#define __XLI_WIN32_WINDOW_H__

#include <Xli/Win32Header.h>
#include <Xli/Window.h>

namespace Xli
{
	/**
		\ingroup WM
	*/
	class Win32Window: public Window
	{
		HWND hWnd;
		bool closed, ownsHwnd;
		
		Shared<WindowEventHandler> eventHandler;
		bool emulateMouseAsTouchPoint, capturedMouseTouchPoint;

		DWORD dwStyle;
		RECT rect;
		bool fullscreen;

		friend class Xli::Window;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		Win32Window(int width, int height, const Xli::String& title, WindowEventHandler* eventHandler, int style);
		Win32Window(HWND hWnd);
		virtual ~Win32Window();

		virtual Platform GetPlatform();

		virtual void Close();
		virtual void RegisterTouchEvents(bool emulateMouseAsTouchPoint);

		virtual bool IsClosed();
		virtual bool IsVisible();
		virtual bool IsFullscreen();
		virtual bool IsMinimized();
		virtual bool IsMaximized();

		virtual int GetDisplayIndex();
		virtual String GetTitle();
		virtual Vector2i GetPosition();
		virtual Vector2i GetClientSize();
		virtual void* GetNativeHandle() { return (void*)hWnd; }

		virtual void SetMainWindow();
		virtual void SetTitle(const String& title);
		virtual void SetFullscreen(bool fullscren);
		virtual void SetPosition(Vector2i pos);
		virtual void SetClientSize(Vector2i size);

		virtual void Minimize();
		virtual void Maximize();
		virtual void Restore();

		virtual void ShowCursor(bool show);

		HWND GetHWND() { return hWnd; }
		void SetIconByID(int id);
	};

}

#endif
