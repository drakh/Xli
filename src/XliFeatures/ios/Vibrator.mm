#include <XliPlatform/PlatformSpecific/iOS.h>
#import <AudioToolbox/AudioServices.h>
#include <XliFeatures.h>

namespace Xli
{
    bool Vibrator::CanVibrate()
    {
        return PlatformSpecific::iOS::IsIPhone();
    }

    bool Vibrator::CanTimedVibrate()
    {
        return false;
    }

    void Vibrator::Vibrate()
    {
        AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);
    }

    void Vibrator::VibrateForMilliseconds(int milliseconds)
    {
        AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);
    }
}
