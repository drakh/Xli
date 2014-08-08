#ifndef __XLI_PLATFORM_SPECIFIC_SDL2_H__
#define __XLI_PLATFORM_SPECIFIC_SDL2_H__

#include <XliPlatform/Window.h>
#include <SDL.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        /**
            \ingroup XliPlatform
        */
        class SDL2
        {
        public:
            static SDL_Window* GetWindowHandle(Window* wnd);
        };
    }
}

#endif
