#include <Xli/Display.h>
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
}
