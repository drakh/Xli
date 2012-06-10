#ifndef __XLI_X11_WINDOW_H__
#define __XLI_X11_WINDOW_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#undef None

#include <XliPlatform/Window.h>

namespace Xli
{
	class X11Window: public Window
	{
		friend class Window;

		::Window wnd;
		int x, y, width, height, lw, lh;
		bool closed;

		Shared<WindowEventHandler> eventHandler;

        WindowStyle style;
		bool sizeable, fs;
		int border;

		void setFullscreen(bool fs);
		void setSizeable(bool sizeable);
		void setBorder(int border);

	public:
		X11Window(int width, int height, const Xli::String& title, WindowEventHandler* eventHandler, WindowStyle style);
		virtual ~X11Window();

		virtual bool IsClosed();
		virtual bool IsActive();
		virtual void* GetNativeHandle();
		virtual Vector2i GetPosition();
		virtual Vector2i GetClientSize();
		virtual WindowStyle GetStyle();

		virtual void SetMainWindow();
		virtual void SetTitle(const String& title);
		virtual void SetStyle(WindowStyle style);
		virtual void SetPosition(Vector2i pos);
		virtual void SetClientSize(Vector2i size);
		virtual void Minimize(bool minimize);
		virtual void Maximize(bool maximize);
		virtual void ShowCursor(bool show);

		virtual void Close();

		// Platform specific
		::Window GetX11Window();
		int GetX11ScreenIndex();

		static ::Display* GetX11Display();
	};
}

#endif
