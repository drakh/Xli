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
        // We could mimic the iphone vibrate by using the following pattern
        // iPhone: 0,200,100,500
        PlatformSpecific::AShim::VibrateForMilliseconds(50);
    }

    void Vibrator::VibrateForMilliseconds(int milliseconds)
    {
        PlatformSpecific::AShim::VibrateForMilliseconds(milliseconds);
    }
}
