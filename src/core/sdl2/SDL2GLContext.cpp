#include <Xli/GLContext.h>
#include <Xli/GLHeaders.h>
#include <Xli/SDL2Window.h>

namespace Xli
{
	class SDL2GLContext: public GLContext
	{
		Shared<SDL2Window> window;
		SDL_GLContext context;

	public:
		SDL2GLContext(SDL2Window* window, int multiSamples)
		{
#ifndef XLI_GLES2
			if (multiSamples > 1)
			{
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multiSamples);
			}
#endif

#ifdef XLI_GLES2
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
            
			this->window = window;
			context = SDL_GL_CreateContext(window->GetSDLWindow());

#ifndef XLI_GLES2
			glewInit();
#endif
            
            Vector2i vp = window->GetClientSize();
            glViewport(0, 0, vp.X, vp.Y);
		}

		virtual ~SDL2GLContext()
		{
            SDL_GL_DeleteContext(context);
		}

		virtual GLContext* CreateSharedContext()
		{
			XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
		}

		virtual int GetMultiSamples()
		{
			int result;
			SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &result);
			return result;
		}

		virtual void SetWindow(Window* window)
		{
			XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
		}

		virtual void MakeCurrent()
		{
			SDL_GL_MakeCurrent(window->GetSDLWindow(), context);
		}

		virtual void SwapBuffers()
		{
			SDL_GL_SwapWindow(window->GetSDLWindow());
		}

		virtual bool SetSwapInterval(int interval)
		{
			return SDL_GL_SetSwapInterval(interval) == 0;
		}
        
		virtual int GetSwapInterval()
		{
			return SDL_GL_GetSwapInterval();
		}
	};

	GLContext* GLContext::Create(Window* window, int multiSamples)
	{
		if (window->GetPlatform() != PlatformSDL2) XLI_THROW("Unsupported window");
		return new SDL2GLContext((SDL2Window*)window, multiSamples);
	}
}
