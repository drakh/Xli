#include <XliPlatform/PlatformSpecific/Android.h>
#include "../../XliPlatform/android/AJniHelper.h"
#include <XliFeatures.h>

namespace Xli
{
    bool Vibrator::CanVibrate()
    {
        return PlatformSpecific::AShim::HasVibrator();
    }

    bool Vibrator::CanTimedVibrate()
    {
        return true;
    }

    void Vibrator::Vibrate()
    {
        PlatformSpecific::AShim::VibrateForMilliseconds(400);
    }

    void Vibrator::VibrateForMilliseconds(int milliseconds)
    {
        PlatformSpecific::AShim::VibrateForMilliseconds(milliseconds);
    }
}
