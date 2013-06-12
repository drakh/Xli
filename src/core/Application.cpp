#include <Xli/Application.h>

namespace Xli
{
	void Run(Application* app)
	{
		// TODO
	}

	int GetInitFlags()
	{
		return WindowFlagsResizeable;
	}

	String GetInitTitle()
	{
		return "Xli Application";
	}

	Vector2i GetInitSize()
	{
#if defined(XLI_PLATFORM_IOS) || defined(XLI_PLATFORM_ANDROID)
		return Window::GetScreenSize();
#else
		return Vector2i(1280, 720);
#endif
	}

	bool OnSizeChanged(Window* wnd, Vector2i size)
	{
#ifdef WIN32
		if (wnd->GetMouseButtonState(MouseButtonLeft))
			Draw();
#endif
		return true;
	}
}
