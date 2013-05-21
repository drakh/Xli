#include <Xli.h>

using namespace Xli;

void PrintPlatformInfo(GLContext* gl)
{
	PrintLine((String)"OpenGL Vendor: " + (const char*)glGetString(GL_VENDOR));
	PrintLine((String)"OpenGL Renderer: " + (const char*)glGetString(GL_RENDERER));
	PrintLine((String)"OpenGL Version: " + (const char*)glGetString(GL_VERSION));
	PrintLine((String)"OpenGL Multisamples: " + gl->GetMultiSamples());
	PrintLine((String)"OpenGL Swap Interval: " + gl->GetSwapInterval());
    
    PrintLine((String)"FileSystem Working Dir: " + Disk->GetCurrentDirectory());
    PrintLine((String)"FileSystem Documents: " + Disk->GetSystemDirectory(SystemDirectoryDocuments));
    PrintLine((String)"FileSystem Local AppData: " + Disk->GetSystemDirectory(SystemDirectoryLocalAppData));
    PrintLine((String)"FileSystem Roaming AppData: " + Disk->GetSystemDirectory(SystemDirectoryRoamingAppData));
    PrintLine((String)"FileSystem Temp Filename: " + Disk->CreateTempFilename());
}

int Main(const Array<String>& args)
{
    PrintLine("Creating GLWindow");

    int flags = WindowStyleResizeable;
    
#if defined(XLI_PLATFORM_IOS) || defined(XLI_PLATFORM_ANDROID)
    flags |= WindowStyleFullscreen;
#endif
    
	Managed<Window> wnd = Window::Create(1280, 720, "GLWindow", 0, flags);
    Managed<GLContext> gl = GLContext::Create(wnd, 16);

	gl->SetSwapInterval(1);
    PrintPlatformInfo(gl);

	while (!wnd->IsClosed())
	{
		glClearColor(1,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// ...
		
		gl->SwapBuffers();
		Window::ProcessMessages();
	}

    PrintLine("Closing GLWindow");
	return 0;
}
