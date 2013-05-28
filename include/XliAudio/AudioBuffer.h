#ifndef __XLI_SOUND_SOUND_H__
#define __XLI_SOUND_SOUND_H__

#include <XliAudio/AudioStream.h>

namespace Xli
{
	/**
		\ingroup Audio
	*/
	class AudioBuffer: public Object
	{
	public:
		virtual DataType GetDataType() const = 0;
		virtual int GetChannelCount() const = 0;
		virtual double GetSampleRate() const = 0;

		virtual double GetLatency() const = 0;

		virtual void Play() = 0;
		virtual void Stop() = 0;


		static void Init();
		static void Done();

		static AudioBuffer* Create(Stream* source, DataType dataType, int channelCount, double sampleRate);
		static AudioBuffer* Create(AudioStream* source);
	};
}

#endif
