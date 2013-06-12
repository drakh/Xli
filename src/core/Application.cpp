#include <Xli/Application.h>

namespace Xli
{
	void Application::Run(Application* app, int flags)
	{
		Managed<Window> wnd = Window::Create(app->GetInitSize(), app->GetInitTitle(), flags);

		app->OnLoad(wnd);
		wnd->SetEventHandler(app);

		while (!wnd->IsClosed())
		{
			app->OnDraw();

			Window::ProcessMessages();
		}
	}

	String Application::GetInitTitle()
	{
		return "Xli Application";
	}

	Vector2i Application::GetInitSize()
	{
#if defined(XLI_PLATFORM_IOS) || defined(XLI_PLATFORM_ANDROID)
		return Window::GetScreenSize();
#else
		return Vector2i(1280, 720);
#endif
	}

	void Application::OnSizeChanged(Window* wnd, Vector2i size)
	{
#ifdef WIN32
		if (wnd->GetMouseButtonState(MouseButtonLeft))
			OnDraw();
#endif
	}
}
