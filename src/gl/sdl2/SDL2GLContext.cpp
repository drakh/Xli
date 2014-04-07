#include <XliGL.h>
#include <Xli/PlatformSpecific/SDL2Window.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        class SDL2GLContext: public GLContext
        {
            Shared<SDL2Window> window;
            SDL_GLContext context;

        public:
            SDL2GLContext(SDL2Window* window, const GLContextAttributes& attribs)
            {
                SDL_GL_SetAttribute(SDL_GL_RED_SIZE, attribs.ColorBits.R);
                SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, attribs.ColorBits.G);
                SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, attribs.ColorBits.B);
                SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, attribs.ColorBits.A);
                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, attribs.DepthBits);
                SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, attribs.StencilBits);
#ifndef XLI_PLATFORM_IOS
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, attribs.Samples > 1 ? 1 : 0);
                SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, attribs.Samples);
#endif
                SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, attribs.AccumBits.R);
                SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, attribs.AccumBits.G);
                SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, attribs.AccumBits.B);
                SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, attribs.AccumBits.A);
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, attribs.Buffers <= 1 ? 0 : 1);
                SDL_GL_SetAttribute(SDL_GL_STEREO, attribs.Stereo ? 1 : 0);

#ifdef XLI_GL_ES2
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

                this->window = window;
                context = SDL_GL_CreateContext(window->GetSDL_Window());

                if (!context)
                    XLI_THROW("Failed to create OpenGL context");

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

            virtual Window* GetWindow()
            {
                return window;
            }

            virtual void SetWindow(Window* window)
            {
                XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
            }

            virtual void MakeCurrent(bool current)
            {
                SDL_GL_MakeCurrent(window->GetSDL_Window(), current ? context : 0);
            }

            virtual bool IsCurrent()
            {
                return SDL_GL_GetCurrentContext() == context;
            }

            virtual void SwapBuffers()
            {
                SDL_GL_SwapWindow(window->GetSDL_Window());
            }

            virtual void SetSwapInterval(int value)
            {
                SDL_GL_SetSwapInterval(value);
            }

            virtual int GetSwapInterval()
            {
                return SDL_GL_GetSwapInterval();
            }

            virtual Vector2i GetDrawableSize()
            {
                Vector2i size;
#ifdef XLI_PLATFORM_LINUX
                SDL_GetWindowSize(window->GetSDL_Window(), &size.X, &size.Y);
#else
                SDL_GL_GetDrawableSize(window->GetSDL_Window(), &size.X, &size.Y);
#endif
                return size;
            }

            virtual void GetAttributes(GLContextAttributes& result)
            {
                memset(&result, 0, sizeof(GLContextAttributes));

                SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &result.ColorBits.R);
                SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &result.ColorBits.G);
                SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &result.ColorBits.B);
                SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &result.ColorBits.A);
                SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &result.DepthBits);
                SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &result.StencilBits);
                SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &result.Samples);
                SDL_GL_GetAttribute(SDL_GL_ACCUM_RED_SIZE, &result.AccumBits.R);
                SDL_GL_GetAttribute(SDL_GL_ACCUM_GREEN_SIZE, &result.AccumBits.G);
                SDL_GL_GetAttribute(SDL_GL_ACCUM_BLUE_SIZE, &result.AccumBits.B);
                SDL_GL_GetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, &result.AccumBits.A);

                int doubleBuffer, stereo;
                SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &doubleBuffer);
                SDL_GL_GetAttribute(SDL_GL_STEREO, &stereo);
                result.Buffers = doubleBuffer ? 2 : 1;
                result.Stereo = stereo ? true : false;
            }
        };
    }

    GLContext* GLContext::Create(Window* window, const GLContextAttributes& attribs)
    {
        if (window->GetImplementation() != WindowImplementationSDL2) 
            XLI_THROW("Unsupported window implementation");

        return new PlatformSpecific::SDL2GLContext((PlatformSpecific::SDL2Window*)window, attribs);
    }
}
