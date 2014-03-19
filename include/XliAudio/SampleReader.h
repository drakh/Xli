#ifndef __XLI_AUDIO_SAMPLE_READER_H__
#define __XLI_AUDIO_SAMPLE_READER_H__

#include <Xli/Stream.h>
#include <XliAudio/AudioStream.h>

namespace Xli
{
    // TODO: Support more input sample formats

    /**
        \ingroup Audio
    */
    class SampleReader
    {
        AudioStream* stream;
        bool forceStereo;
        int internalBufferSize;
        Int16* internalBuffer;

        static float convertSample(Int16 sample);

    public:
        SampleReader(AudioStream* stream, bool forceStereo);
        ~SampleReader();

        int Read(float* buffer, int expectedSamples);
    };
}

#endif
