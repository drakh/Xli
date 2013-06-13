#ifndef __XLI_AUDIO_AUDIO_STREAM_H__
#define __XLI_AUDIO_AUDIO_STREAM_H__

#include <Xli/Stream.h>
#include <Xli/DataType.h>

namespace Xli
{
	/**
		\ingroup Audio
	*/
	class AudioStream: public Stream
	{
	public:
		virtual double GetSampleRate() const = 0;
		virtual int GetChannelCount() const = 0;
		virtual DataType GetDataType() const = 0;
		virtual int GetAverageBytesPerSecond() const;

		static AudioStream* Create(Stream* source, DataType dataType, int channelCount, double sampleRate);
	};
}

#endif
