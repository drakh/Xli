#include <XliVibrator.h>

namespace Xli
{
    bool Vibrator::CanVibrate() { return false; }
    bool Vibrator::CanTimedVibrate() { return false; }
    void Vibrator::Vibrate() {}
    void Vibrator::VibrateForMilliseconds(int milliseconds) {}
}
