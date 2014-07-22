#include <XliPlatform/Display.h>
#include <SDL.h>

namespace Xli
{
    int Display::GetCount()
    {
        return SDL_GetNumVideoDisplays();
    }

    Recti Display::GetRect(int index)
    {
        SDL_Rect rect;
        SDL_GetDisplayBounds(index, &rect);
        return Recti(Vector2i(rect.x, rect.y), Vector2i(rect.w, rect.h));
    }

#if !defined(XLI_PLATFORM_OSX) && !defined(XLI_PLATFORM_IOS)
    float Display::GetDensity(int displayIndex)
    {
        // TODO
        return 1.0f;
    }

    Vector2 Display::GetDpi(int displayIndex)
    {
        // TODO
        return Vector2(72, 72);
    }
#endif
}
