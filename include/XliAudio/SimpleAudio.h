#ifndef __XLI_AUDIO_SIMPLE_AUDIO_PLAYER_H__
#define __XLI_AUDIO_SIMPLE_AUDIO_PLAYER_H__

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <Xli/Object.h>

namespace Xli
{
	class SimpleSoundChannel: public Object
	{
	public:
        virtual int GetPosition() const = 0;
        virtual void SetPosition(int position) = 0;
		virtual float GetPan() const = 0;
		virtual void SetPan(float pan) const = 0;        
		virtual float GetVolume() const = 0;
		virtual void SetVolume(float volume) const = 0;
        virtual void Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;
		virtual bool IsPaused() = 0;
		virtual bool IsPlaying() = 0;
		virtual bool IsFinished() = 0;
        
        static SimpleSoundChannel* Create(const String& path, bool androidAsset);
	};
}

#endif
