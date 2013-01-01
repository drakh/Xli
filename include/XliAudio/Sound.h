#ifndef __XLI_SOUND_SOUND_H__
#define __XLI_SOUND_SOUND_H__

#include <Xli/DataType.h>
#include <Xli/Stream.h>

namespace Xli
{
	class Sound: public Object
	{
	public:
		virtual DataType GetDataType() const = 0;
		virtual int GetChannelCount() const = 0;
		virtual double GetSampleRate() const = 0;

		virtual double GetLatency() const = 0;

		virtual void Play() = 0;
		virtual void Stop() = 0;


		static void Init();
		static void Shutdown();

		static Sound* Create(Stream* source, DataType dataType, int channels, double sampleRate);
	};
}

#endif
