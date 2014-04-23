#include <Xli/Display.h>
#include <Xli/PlatformSpecific/iOS.h>
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

#ifdef XLI_PLATFORM_IOS
    float Display::GetDensity(int displayIndex)
    {
        PlatformSpecific::IDeviceType dType = PlatformSpecific::GetIDeviceType();
        return PlatformSpecific::GetIDeviceDensity(dType);
    }
    Vector2 Display::GetDpi(int displayIndex)
    {
        return Vector2(0.0f, 0.0f);
    }
#endif
}
