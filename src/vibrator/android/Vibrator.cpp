#include <Xli/PlatformSpecific/Android.h>
#include "../../core/android/AJniHelper.h"
#include <XliVibrator.h>

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
        PlatformSpecific::AShim::VibrateForMilliseconds(50);
    }

    void Vibrator::VibrateForMilliseconds(int milliseconds)
    {
        PlatformSpecific::AShim::VibrateForMilliseconds(milliseconds);
    }
}
