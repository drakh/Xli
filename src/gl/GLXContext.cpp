#include <XliGL/GLContext.h>
#include <XliPlatform/X11Window.h>

#include <GL/glxew.h>
#include <GL/glx.h>

namespace Xli
{
	class GLXContext: public GLContext
	{
	    ::GLXContext ctx;
	    Shared<X11Window> wnd;
		XVisualInfo* vi;
		bool ownsVi;
		
	public:
		GLXContext(X11Window* wnd, int multiSamples)
		{
		    this->wnd = wnd;

			// TODO: Multisampling is not supported yet.

            int attrList[] =
            {
                GLX_RGBA,
                GLX_RED_SIZE, 4,
                GLX_GREEN_SIZE, 4,
                GLX_BLUE_SIZE, 4,
                GLX_DEPTH_SIZE, 16,
                //GLX_STENCIL_SIZE, 8,
                GLX_DOUBLEBUFFER,
                0
            };

			int screen = wnd->GetX11ScreenIndex();

            vi = glXChooseVisual(wnd->GetX11Display(), screen, attrList);
			ownsVi = true;

            if (!vi)
            {
                attrList[9] = 0;
                vi = glXChooseVisual(wnd->GetX11Display(), screen, attrList);

                if (!vi)
                {
                    XLI_THROW("Unable to choose X visual");
                }

                ErrorPrintLine("GL WARNING: Running single buffered");
            }

            ctx = glXCreateContext(wnd->GetX11Display(), vi, 0, GL_TRUE);

			if (!ctx)
			{
				XLI_THROW("Failed to create OpenGL context");
			}
			
            MakeCurrent();
            glewInit();
		}

		GLXContext(GLXContext* src_ctx)
		{
			this->wnd = src_ctx->wnd;
			this->vi = src_ctx->vi;
			ownsVi = false;
			
            /*if (!glXMakeCurrent(wnd->GetX11Display(), 0, 0))
			{
				XLI_THROW("Unable to make OpenGL context not current");
			}*/
			
			this->ctx = glXCreateContext(wnd->GetX11Display(), vi, src_ctx->ctx, GL_TRUE);
			
			if (this->ctx == NULL)
			{
				XLI_THROW("Failed to create shared OpenGL context");
			}
			
			src_ctx->MakeCurrent();
		}

		virtual ~GLXContext()
		{
            glXDestroyContext(wnd->GetX11Display(), ctx);
			if (ownsVi) XFree(vi);
		}

		virtual GLContext* CreateSharedContext()
		{
			return new GLXContext(this);
		}

        virtual void MakeCurrent()
        {
            if (!glXMakeCurrent(wnd->GetX11Display(), wnd->GetX11Window(), ctx))
			{
				XLI_THROW("Unable to make OpenGL context current");
			}
        }

		virtual void SwapBuffers()
		{
            glXSwapBuffers(wnd->GetX11Display(), wnd->GetX11Window());
		}
		
		virtual bool SetSwapInterval(int interval)
		{
			return glXSwapIntervalSGI(interval) == 0;
		}
		
		virtual int GetMultiSamples()
		{
			return 0;
		}
	};

	GLContext* GLContext::Create(Window* window, int multiSamples)
	{
		if (window->GetPlatform() != PlatformX11) XLI_THROW("Unsupported window");
		return new GLXContext((X11Window*)window, multiSamples);
	}
}
