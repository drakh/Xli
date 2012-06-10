#include <XliPlatform/Display.h>
#include <XliPlatform/X11Window.h>
#include <cstdlib>

namespace Xli
{
	static int initCount = 0;

	void Display::Init()
	{
		/*if (!initCount)
		{
		}

		initCount++;*/
	}

	void Display::Shutdown()
	{
		/*initCount--;

		if (initCount == 0)
		{
		}
		else if (initCount < 0)
		{
			XLI_THROW_BAD_DELETE;
		}*/
	}

	static void AssertInit()
	{
		/*if (!initCount)
		{
			Display::Init();
			atexit(Screen::Shutdown);
		}*/
	}

	int Display::GetCount()
	{
		//AssertInit();

		return ScreenCount(X11Window::GetX11Display());
	}

	Recti Display::GetRect(int index)
	{
		//AssertInit();

		// BUG: This method can on some configurations return a rect spanning all monitors on multimonitor systems.
		// TODO: Should return one rect per monitor.

		::Display* x_display = X11Window::GetX11Display();
		::Window root = RootWindow(x_display, index);
		
		XWindowAttributes attr;
		XGetWindowAttributes(x_display, root, &attr);

		return Recti(Vector2i(attr.x, attr.y), Vector2i(attr.width, attr.height));
	}
}
