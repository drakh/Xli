#include <XliVibrator.h>

namespace Xli
{
    bool Vibrator::CanVibrate() { return false; }
    bool Vibrator::CanTimedVibrate() { return false; }
    void Vibrator::Vibrate() { XLI_THROW("Xli: Vibration not supported on this platform") }
    void Vibrator::VibrateForMilliseconds(int milliseconds) {XLI_THROW("Xli: Vibration not supported on this platform") }
}
