#ifndef __XLI_FEATURES_VIBRATOR_H__
#define __XLI_FEATURES_VIBRATOR_H__

namespace Xli
{   
    class Vibrator
    { 
    public:
        static bool CanVibrate();
        static bool CanTimedVibrate();
        static void Vibrate();
        static void VibrateForMilliseconds(int milliseconds);
    };
}

#endif
