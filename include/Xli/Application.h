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
		static void Run(Application* app);

		virtual void Load(Window* wnd) = 0;
		virtual void Draw() = 0;

		virtual int GetInitFlags();
		virtual String GetInitTitle();
		virtual Vector2i GetInitSize();

		virtual bool OnSizeChanged(Window* wnd, Vector2i size);
	};
}

#endif
