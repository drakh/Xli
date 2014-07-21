#ifndef __XLI_PLATFORM_DISPLAY_H__
#define __XLI_PLATFORM_DISPLAY_H__

#include <Xli/Rectangle.h>

namespace Xli
{
    /**
        \ingroup XliPlatform
    */
    struct DisplaySettings
    {
        Vector2i Resolution;
        int RefreshRate;
        int BitsPerPixel;
    };

    /**
        \ingroup XliPlatform
    */
    class Display
    {
    public:
        /**
            Returns the number of screens connected to the system.
        */
        static int GetCount();

        /**
            Returns the coordinate system of the specified screen.
        */
        static Recti GetRect(int index);

        static bool GetCurrentSettings(int index, DisplaySettings& settings);
        static void GetSupportedSettings(int index, Array<DisplaySettings>& settings);
        static bool ChangeSettings(int index, const DisplaySettings& settings);

        static float GetDensity(int displayIndex);
        static Vector2 GetDpi(int displayIndex);
    };
}


#endif
