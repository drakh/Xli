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

#include <XliPlatform/Window.h>
#include <Xli/Thread.h>
#include <Xli/StringBuilder.h>
#include <cstring>

namespace Xli
{
    bool WindowEventHandler::OnKeyDown(Window* wnd, Key key)
    {
        return false;
    }

    bool WindowEventHandler::OnKeyUp(Window* wnd, Key key)
    {
        return false;
    }

    bool WindowEventHandler::OnTextInput(Window* wnd, const String& text)
    {
        return false;
    }
    
    bool WindowEventHandler::OnMouseDown(Window* wnd, Vector2i pos, MouseButton button)
    {
        return false;
    }

    bool WindowEventHandler::OnMouseUp(Window* wnd, Vector2i pos, MouseButton button)
    {
        return false;
    }

    bool WindowEventHandler::OnMouseMove(Window* wnd, Vector2i pos)
    {
        return false;
    }

    bool WindowEventHandler::OnMouseWheel(Window* wnd, Vector2i delta)
    {
        return false;
    }
    
    bool WindowEventHandler::OnTouchDown(Window* wnd, Vector2 pos, int id)
    {
        return false;
    }

    bool WindowEventHandler::OnTouchMove(Window* wnd, Vector2 pos, int id)
    {
        return false;
    }

    bool WindowEventHandler::OnTouchUp(Window* wnd, Vector2 pos, int id)
    {
        return false;
    }
    
    void WindowEventHandler::OnNativeHandleChanged(Window* wnd)
    {
    }

    void WindowEventHandler::OnSizeChanged(Window* wnd)
    {
    }

    bool WindowEventHandler::OnClosing(Window* wnd)
    {
        return false;
    }

    void WindowEventHandler::OnClosed(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppLowMemory(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppTerminating(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppWillEnterForeground(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppDidEnterForeground(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppWillEnterBackground(Window* wnd)
    {
    }

    void WindowEventHandler::OnAppDidEnterBackground(Window* wnd)
    {
    }
}
