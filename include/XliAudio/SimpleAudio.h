#ifndef __XLI_AUDIO_SIMPLE_AUDIO_PLAYER_H__
#define __XLI_AUDIO_SIMPLE_AUDIO_PLAYER_H__

#include <Xli/Object.h>

namespace Xli
{
	class SimpleSoundChannel : public Object
	{
	public:
		virtual void Pause() = 0;
        virtual void UnPause() = 0;
        virtual double GetPosition() const = 0;
        virtual void SetPosition(double position) = 0;
		virtual float GetVolume() const = 0;
		virtual void SetVolume(float volume) const = 0;
		virtual bool IsPlaying() = 0;
		virtual bool IsFinished() = 0;

        virtual double GetDuration() const = 0;
		virtual float GetPan() const = 0;
		virtual void SetPan(float pan) const = 0;
		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual bool IsPaused() = 0;
	};

    class SimpleSound : public Object
    {
    public:
        virtual double GetDuration() const = 0;
        virtual SimpleSoundChannel* Play(bool paused) = 0;
        virtual SimpleSoundChannel* PlayLoop(bool paused) = 0;
        virtual String GetPath() const = 0;
        virtual bool IsAsset() const = 0;
        static SimpleSound* Create(const String& path, bool asset);
    };
}

#endif
