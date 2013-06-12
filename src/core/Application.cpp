#include <Xli/Application.h>

namespace Xli
{
	void Application::Run(Application* app, int flags)
	{
		Window::Init();

		Managed<Window> wnd = Window::Create(app->GetInitSize(), app->GetInitTitle(), flags);

		app->OnInit(wnd);
		wnd->SetEventHandler(app);
		
		app->OnLoad(wnd);

		while (!wnd->IsClosed())
		{
			app->OnDraw(wnd);
			Window::ProcessMessages();
		}

		Window::Done();
	}

	void Application::OnInit(Window* wnd)
	{
	}
	
	void Application::OnLoad(Window* wnd)
	{
	}

	void Application::OnDraw(Window* wnd)
	{
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
			OnDraw(wnd);
#endif
	}
}
