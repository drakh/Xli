#ifndef __XLI_SOUND_H__
#define __XLI_SOUND_H__

#include <Xli/Object.h>
#include <Xli/String.h>
#include <XliAudio/SoundChannel.h>

namespace Xli
{
	/**
		\ingroup Audio
	*/
	enum SoundPlayHint
	{
		SoundPlayHintDefault,
		SoundPlayHintStream,
	};

	/**
		\ingroup Audio
	*/
	class Sound: public Object
	{
	public:
		virtual double GetDuration() = 0;

		virtual SoundChannel* Play() = 0;
		virtual SoundChannel* PlayLoop() = 0;

		static Sound* Create(const String& filename, SoundPlayHint hint = SoundPlayHintDefault);
	};
}

#endif
