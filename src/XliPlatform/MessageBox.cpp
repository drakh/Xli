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

#include <XliPlatform/MessageBox.h>
#include <XliPlatform/Window.h>
#include <Xli/Thread.h>
#include <Xli/StringBuilder.h>

namespace Xli
{
    void MessageBox::HandleException(const Exception& e, const String& catcher, Window* wnd)
    {
        StringBuilder sb(2048);

        sb.Append(e.GetMessage());

        sb.Append("\n\nFunction: ");
        sb.Append(e.GetFunction());
        sb.Append("\nLine: ");
        sb.Append(String(e.GetLine()));

        if (catcher.Length())
        {
            sb.Append("\n\nCaught by: ");
            sb.Append(catcher);
        }

        if (!wnd)
            wnd = Window::GetMainWindow();

        Show(wnd, sb, "Unhandled exception", Xli::DialogButtonsOK, Xli::DialogHintError);
    }
}
