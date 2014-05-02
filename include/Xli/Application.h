#ifndef __XLI_APPLICATION_H__
#define __XLI_APPLICATION_H__

#include <Xli/Window.h>

namespace Xli
{
    /**
        \ingroup XliCorePlatform
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

        virtual void OnSizeChanged(Window* wnd, Vector2i size);
    };
}

#endif
