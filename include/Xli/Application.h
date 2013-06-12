#ifndef __XLI_APPLICATION_H__
#define __XLI_APPLICATION_H__

#include <Xli/Window.h>

namespace Xli
{
	/**
		\ingroup WM
	*/
	class Application: public WindowEventHandler
	{
	public:
		static void Run(Application* app, int flags = WindowFlagsResizeable);

		virtual void OnLoad(Window* wnd) = 0;
		virtual void OnDraw() = 0;

		virtual String GetInitTitle();
		virtual Vector2i GetInitSize();

		virtual void OnSizeChanged(Window* wnd, Vector2i size);
	};
}

#endif
