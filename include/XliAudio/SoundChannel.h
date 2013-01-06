#ifndef __XLI_SOUND_CHANNEL_H__
#define __XLI_SOUND_CHANNEL_H__

#include <Xli/Object.h>

namespace Xli
{
	/**
		\ingroup Audio
	*/
	class SoundChannel: public Object
	{
	public:
		virtual double GetPosition() = 0;
		virtual void SetPosition(double value) = 0;

		virtual float GetPan() = 0;
		virtual void SetPan(float value) = 0;

		virtual float GetVolume() = 0;
		virtual void SetVolume(float value) = 0;

		virtual bool GetPaused() = 0;
		virtual void SetPaused(bool value) = 0;

		virtual bool IsPlaying() = 0;
		virtual bool IsFinished() = 0;

		virtual void Stop() = 0;

		virtual double GetDuration() = 0;

		virtual void Play() = 0;
		virtual void PlayLoop() = 0;
	};
}

#endif
