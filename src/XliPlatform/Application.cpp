//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <XliPlatform/Application.h>
#include <XliPlatform/Display.h>
#include <XliPlatform/PlatformLib.h>
#include <Xli/Managed.h>
#include <Xli/Thread.h>
#include <Xli/Time.h>

namespace Xli
{
    void Application::Run(Application* app, int flags)
    {
        Managed<Window> wnd = Window::Create(app->GetInitSize(), app->GetInitTitle(), flags);

        app->OnInit(wnd);
        wnd->SetEventHandler(app);
        Window::ProcessMessages();

        app->OnLoad(wnd);

        while (!wnd->IsClosed())
        {
            double startTime = Xli::GetSeconds();

            Window::ProcessMessages();
            app->OnUpdate(wnd);

            if (wnd->IsVisible())
                app->OnDraw(wnd);

            if (app->_maxFps > 0)
            {
                double targetTime = 1.0 / (double)app->_maxFps;
                double renderTime = GetSeconds() - startTime;

                int msTimeout = (int)((targetTime - renderTime) * 1000.0 + 0.5);

                if (msTimeout > 0)
                    Sleep(msTimeout);
            }
        }
    }

    Application::Application()
    {
        PlatformLib::Init();

        DisplaySettings settings;
        if (Display::GetCount() > 0 && 
            Display::GetCurrentSettings(0, settings) && 
            settings.RefreshRate > 0)
            _maxFps = settings.RefreshRate;
        else
            _maxFps = 60;
    }
    
    void Application::SetMaxFps(int value)
    {
        _maxFps = value;
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

    void Application::OnInit(Window* wnd)
    {
    }
    
    void Application::OnLoad(Window* wnd)
    {
    }

    void Application::OnUpdate(Window* wnd)
    {
    }

    void Application::OnDraw(Window* wnd)
    {
    }

    void Application::OnSizeChanged(Window* wnd)
    {
#if defined(XLI_PLATFORM_WIN32) || defined(XLI_PLATFORM_OSX)
        if (wnd->GetMouseButtonState(MouseButtonLeft))
            OnDraw(wnd);
#endif
    }
}
