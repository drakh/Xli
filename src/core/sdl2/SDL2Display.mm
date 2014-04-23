#include <Xli/Console.h>
#include <Xli/Display.h>
#include <Xli/PlatformSpecific/iOS.h>
#include <SDL.h>

namespace Xli
{
    float Display::GetDensity(int displayIndex)
    {
        PlatformSpecific::IDeviceType dType = PlatformSpecific::GetIDeviceType();
        float dpi = PlatformSpecific::GetIDeviceDpi(dType);
        // the scale factor is to bring it in line with android.
        return dpi / 160.0f;
    }
    Vector2 Display::GetDpi(int displayIndex)
    {
        PlatformSpecific::IDeviceType dType = PlatformSpecific::GetIDeviceType();
        float dpi = PlatformSpecific::GetIDeviceDpi(dType);
        return Vector2(dpi, dpi);
    }
}
