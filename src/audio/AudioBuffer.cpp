#include <XliAudio/AudioBuffer.h>

namespace Xli
{
    AudioBuffer* AudioBuffer::Create(AudioStream* source)
    {
        return Create(source, source->GetDataType(), source->GetChannelCount(), source->GetSampleRate());
    }
}
