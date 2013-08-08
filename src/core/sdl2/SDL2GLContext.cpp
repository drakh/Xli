#include <XliGL.h>
#include <Xli/GLContext.h>
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
#ifdef XLI_GL_DESKTOP
			if (multiSamples > 1)
			{
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multiSamples);
			}
#endif

#ifdef XLI_GL_ES2
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

			this->window = window;
			context = SDL_GL_CreateContext(window->GetSDL_Window());

			if (!context)
			{
			    XLI_THROW("Failed to create OpenGL context");
			}

            Vector2i vp = window->GetClientSize();
            glViewport(0, 0, vp.X, vp.Y);

#ifdef XLI_GL_DESKTOP
			glewInit();
#endif
		}

		virtual ~SDL2GLContext()
		{
            SDL_GL_DeleteContext(context);
		}

		virtual GLContext* CreateSharedContext()
		{
			XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
		}

		virtual int GetMultiSamples()
		{
			int result;
			SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &result);
			return result;
		}

		virtual void SetWindow(Window* window)
		{
			XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
		}

		virtual void MakeCurrent(bool current)
		{
			SDL_GL_MakeCurrent(window->GetSDL_Window(), current ? context : 0);
		}

		virtual void SwapBuffers()
		{
			SDL_GL_SwapWindow(window->GetSDL_Window());
		}

		virtual bool SetSwapInterval(int interval)
		{
			return SDL_GL_SetSwapInterval(interval) == 0;
		}

		virtual int GetSwapInterval()
		{
			return SDL_GL_GetSwapInterval();
		}

		virtual unsigned int GetBackbufferHandle()
		{
#ifdef XLI_PLATFORM_IOS
			return 1;
#else
			return 0;
#endif
		}
	};

	GLContext* GLContext::Create(Window* window, int multiSamples)
	{
		if (window->GetImplementation() != WindowImplementationSDL2) XLI_THROW("Unsupported window");
		return new SDL2GLContext((SDL2Window*)window, multiSamples);
	}
}
