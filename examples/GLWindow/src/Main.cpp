#include <Xli.h>

using namespace Xli;

int Main(const Array<String>& args)
{
    PrintLine("Creating GLWindow");
    
	Managed<Window> wnd = Window::Create(1280, 720, "GLWindow", 0, WindowStyleResizeable);
    Managed<GLContext> glc = GLContext::Create(wnd, 16);

	while (!wnd->IsClosed())
	{
		glClearColor(1,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glc->SwapBuffers();
		Window::ProcessMessages();
	}

    PrintLine("GLWindow was closed");
    
	return 0;
}
