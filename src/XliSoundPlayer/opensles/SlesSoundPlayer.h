#ifndef __XLI_SLESSOUNDPLAYER_ANDROID_H__
#define __XLI_SLESSOUNDPLAYER_ANDROID_H__

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <XliSoundPlayer.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        class SlesAudioEngine : public Object
        {
        public:
            SLObjectItf EngineObject;
            SLEngineItf EngineEngine;
            SLObjectItf OutputMixObject;
        };
    }
};

#endif
