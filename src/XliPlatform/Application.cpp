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
#include <XliPlatform/PlatformLib.h>
#include <Xli/Managed.h>

namespace Xli
{
    void Application::Run(Application* app, int flags)
    {
        PlatformLib::Init();
        Managed<Window> wnd = Window::Create(app->GetInitSize(), app->GetInitTitle(), flags);

        app->OnInit(wnd);
        wnd->SetEventHandler(app);
        Window::ProcessMessages();

        app->OnLoad(wnd);

        while (!wnd->IsClosed())
        {
            app->OnDraw(wnd);
            Window::ProcessMessages();
        }
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

    void Application::OnSizeChanged(Window* wnd)
    {
#if defined(XLI_PLATFORM_WIN32) || defined(XLI_PLATFORM_OSX)
        if (wnd->GetMouseButtonState(MouseButtonLeft))
            OnDraw(wnd);
#endif
    }
}
