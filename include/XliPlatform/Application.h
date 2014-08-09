/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_APPLICATION_H__
#define __XLI_APPLICATION_H__

#include <XliPlatform/Window.h>

namespace Xli
{
    /**
        \ingroup XliPlatform
    */
    class Application: public WindowEventHandler
    {
    public:
        static void Run(Application* app, int flags = WindowFlagsResizeable);

        virtual void OnInit(Window* wnd);
        virtual void OnLoad(Window* wnd);
        virtual void OnDraw(Window* wnd);

        virtual String GetInitTitle();
        virtual Vector2i GetInitSize();

        virtual void OnSizeChanged(Window* wnd);
    };
}

#endif
