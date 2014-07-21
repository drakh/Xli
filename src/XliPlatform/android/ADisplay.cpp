#include "3rdparty/android_native_app_glue.h"
#include <android/window.h>
#include <XliPlatform/PlatformSpecific/Android.h>
#include <XliPlatform/Display.h>
#include <XliPlatform/Window.h>
#include "AShim.h"

namespace Xli
{
    int Display::GetCount()
    {
        return 1;
    }

    Recti Display::GetRect(int index)
    {
        Vector2i size = Window::GetScreenSize();
        return Recti(0, 0, size.X, size.Y);
    }

    bool Display::GetCurrentSettings(int index, DisplaySettings& settings)
    {
        return false;
    }

    void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings)
    {
    }

    bool Display::ChangeSettings(int index, const DisplaySettings& settings)
    {
        return false;
    }

    float Display::GetDensity(int displayIndex)
    {
        return PlatformSpecific::AShim::GetDensity();
    }
    Vector2 Display::GetDpi(int displayIndex)
    {
        return PlatformSpecific::AShim::GetDpi();
    }
}
