#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <XliAudio/SimpleAudio.h>
#include <Xli/PlatformSpecific/Android.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <assert.h>

extern Xli::PlatformSpecific::SlesAudioEngine* GlobalAAudioEngine = 0;

//{TODO} share the player obj

namespace Xli
{
    
    class SlesSimpleSoundChannel : public SimpleSoundChannel
    {
    private:
        SLObjectItf playerObject;
        SLPlayItf playerPlayItf;
        SLSeekItf playerSeekItf;
        SLVolumeItf playerVolumeItf;
        bool Closed;
        mutable double cachedDuration;
        
    public:
        SlesSimpleSoundChannel(const SimpleSound& sound)
        {
            playerObject = NULL;
            playerPlayItf = NULL;
            playerSeekItf = NULL;
            playerVolumeItf = NULL;
            cachedDuration = -1.0;
            Closed = !PrepareSlesPlayer(sound.GetPath(), sound.IsAsset());
        }    

        virtual ~SlesSimpleSoundChannel()
        {
            playerPlayItf = NULL;
            playerSeekItf = NULL;
            playerVolumeItf = NULL;
            (*playerObject)->Destroy(playerObject);
            Closed=true;
        }

		virtual void Pause()
        {
            (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PAUSED);
        }
        virtual void UnPause()
        {
            if (this->IsPaused())
                (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PLAYING);
        }

        virtual double GetPosition() const
        {
            SLmillisecond pos = -1;
            SLresult result = (*playerPlayItf)->GetPosition(playerPlayItf, &pos);
            assert(SL_RESULT_SUCCESS == result);
            return ((double)((int)pos))/1000.0;
        }
        virtual void SetPosition(double position)
        {
            int pos = (int)(position * 1000);
            SLresult result = (*playerSeekItf)->SetPosition(playerSeekItf, pos, SL_SEEKMODE_FAST);
            assert(SL_RESULT_SUCCESS == result);
        }

        float GainToAttenuation(float gain) const
        {
            //http://vec3.ca/getting-started-with-opensl-on-android/
            return gain < 0.01f ? -96.0f : 20 * log10( gain );
        }
        float AttenuationToGain(float att) const
        {
            return att <= -96.0f ? 0.0f : pow(10, (att / 20.0f));
        }
		virtual float GetVolume() const
        {
            SLmillibel vol;
            SLresult result = (*playerVolumeItf)->GetVolumeLevel(playerVolumeItf, &vol);
            assert(SL_RESULT_SUCCESS == result);
            return AttenuationToGain(vol / 100.0f);
        }
		virtual void SetVolume(float volume) const
        {
            float att = (GainToAttenuation(volume) * 100.0f);
            SLresult result = (*playerVolumeItf)->SetVolumeLevel(playerVolumeItf, (SLmillibel)att);
            assert(SL_RESULT_SUCCESS == result);
        }

		virtual bool IsPlaying()
        {
            SLuint32 pState;
            SLresult result = (*playerPlayItf)->GetPlayState(playerPlayItf, &pState);
            return (pState == SL_PLAYSTATE_PLAYING);
        }

		virtual bool IsFinished()
        {
            return ((!IsPlaying()) && AtEnd());
        }

        virtual double GetDuration() const
        {
            if (cachedDuration<0)
            {
                SLmillisecond duration = -1;
                SLresult result = (*playerPlayItf)->GetDuration(playerPlayItf, &duration);
                assert(SL_RESULT_SUCCESS == result);
                cachedDuration = ((double)((int)duration)/1000.0);
            } 
            return cachedDuration;
        }
 
        virtual void Play()
        {
            (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PLAYING);
        }
		virtual void Stop()
        {
            SLresult result=(*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_STOPPED);
            assert(SL_RESULT_SUCCESS == result);
        }

        virtual bool GetLoop()
        {
            SLboolean looping;
            SLmillisecond milli_start;
            SLmillisecond milli_end;
            SLresult result = (*playerSeekItf)->GetLoop(playerSeekItf, &looping,
                                                        &milli_start, &milli_end);
            assert(SL_RESULT_SUCCESS == result);
            return (bool)looping;
        }
        virtual void SetLoop(double milli_start, double milli_end)
        {                     
            SLresult result = (*playerSeekItf)->SetLoop(playerSeekItf, SL_BOOLEAN_TRUE,
                                                        (SLmillisecond)milli_start,
                                                        (SLmillisecond)milli_end);
            assert(SL_RESULT_SUCCESS == result);
        }

		virtual float GetPan() const
        {
            SLpermille pos;
            SLresult result = (*playerVolumeItf)->GetStereoPosition(playerVolumeItf, &pos);
            assert(SL_RESULT_SUCCESS == result);
            float pan = pos / 1000.0f;
            if (pan < -1.0f) pan = -1.0f;
            if (pan > 1.0f) pan = 1.0f;
            return pan;
        }
		virtual void SetPan(float pan) const
            {
                if (pan < -1.0f) pan = -1.0f;
                if (pan > 1.0f) pan = 1.0f;
                SLpermille pos = pan * 1000.0f;
                SLresult result = (*playerVolumeItf)->SetStereoPosition(playerVolumeItf, pos);
                assert(SL_RESULT_SUCCESS == result);
            }

		virtual bool IsPaused()
        {
            SLuint32 pState;
            SLresult result = (*playerPlayItf)->GetPlayState(playerPlayItf, &pState);
            assert(SL_RESULT_SUCCESS == result);
            return (pState == SL_PLAYSTATE_PAUSED);
        }

        virtual bool IsClosed() const
        {
            return Closed;
        }

        virtual bool AtEnd() const
        {
            int dur = GetDuration();
            int pos = GetPosition();
            return (dur == pos);
        }

        virtual bool PrepareSlesPlayer(const String& path, bool asset)
        {            
            SLresult result;
            const SLInterfaceID ids[2] = {SL_IID_SEEK, SL_IID_VOLUME};
            const SLboolean req[2] = {SL_BOOLEAN_FALSE, SL_BOOLEAN_TRUE};
            // configure audio source
            if (asset) {
                Xli::PlatformSpecific::AJniHelper jni;
                AAssetManager* mgr = Xli::PlatformSpecific::AShim::GetAssetManager();
                assert(NULL != mgr);
                AAsset* assetp = AAssetManager_open(mgr, path.DataPtr(), AASSET_MODE_UNKNOWN);
                if (NULL == assetp) {
                    XLI_THROW("****Cant find asset****");
                    return false;
                }
                off_t start, length;
                int fd = AAsset_openFileDescriptor(assetp, &start, &length);
                assert(0 <= fd);
                AAsset_close(assetp);
                SLDataLocator_AndroidFD locatorIn = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
                SLDataFormat_MIME dataFormat = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
                SLDataSource audioSrc = {&locatorIn, &dataFormat};
                SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, 
                                                      GlobalAAudioEngine->OutputMixObject};
                SLDataSink audioSnk = {&loc_outmix, NULL};

                result = (*GlobalAAudioEngine->EngineEngine)->CreateAudioPlayer(GlobalAAudioEngine->EngineEngine, &playerObject, &audioSrc, &audioSnk, 2, ids, req);
                assert(SL_RESULT_SUCCESS == result);
            } else {

                SLDataLocator_URI locatorIn = {SL_DATALOCATOR_URI, (SLchar *)path.DataPtr()};
                SLDataFormat_MIME dataFormat = {SL_DATAFORMAT_MIME, NULL, 
                                                SL_CONTAINERTYPE_UNSPECIFIED};
                SLDataSource audioSrc = {&locatorIn, &dataFormat};
                SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, 
                                                      GlobalAAudioEngine->OutputMixObject};
                SLDataSink audioSnk = {&loc_outmix, NULL};

                result = (*GlobalAAudioEngine->EngineEngine)->CreateAudioPlayer(GlobalAAudioEngine->EngineEngine, &playerObject, &audioSrc, &audioSnk, 2, ids, req);
                assert(SL_RESULT_SUCCESS == result);
            }

            (void)result;
            // realize the player
            result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            // get the play interface
            result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlayItf);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            // get the seek interface
            result = (*playerObject)->GetInterface(playerObject, SL_IID_SEEK,
                                                   &playerSeekItf);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            // get the volume interface
            result = (*playerObject)->GetInterface(playerObject, SL_IID_VOLUME,
                                                   &playerVolumeItf);
            (*playerVolumeItf)->EnableStereoPosition(playerVolumeItf, SL_BOOLEAN_TRUE);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PAUSED);
            return true;
        }
    };
   
    class SlesSimpleSound : public SimpleSound 
    {
    private:
        double duration;
        String path;
        bool isasset;
    public:
        SlesSimpleSound(const String& path, bool asset)
        {
            duration = -1;
            this->path = path;
            this->isasset = asset;
            PopulateMetadata();
        }
        virtual double GetDuration() const
        {
            return duration;
        }
        virtual void PopulateMetadata()
        {
            SlesSimpleSoundChannel* channel = new SlesSimpleSoundChannel(*this);
            duration = channel->GetDuration();
            delete(channel);
        }
        virtual SimpleSoundChannel* Play(bool paused)
        {
            SlesSimpleSoundChannel* result = new SlesSimpleSoundChannel(*this);
            if (!paused) result->Play();
            return (SimpleSoundChannel*)result;
        }
        virtual SimpleSoundChannel* PlayLoop(bool paused)
        {
            SlesSimpleSoundChannel* result = new SlesSimpleSoundChannel(*this);
            if (paused) result->Pause();
            result->SetLoop(0, duration);
            return (SimpleSoundChannel*)result;
        }
        virtual String GetPath() const
        {
            return path;
        }
        virtual bool IsAsset() const
        {
            return isasset;
        }
    };

    class SimpleAudioEngine
    {
    public:
        static void Startup()
        {
            SLresult result;
            Xli::PlatformSpecific::SlesAudioEngine* newAudioEngine = new Xli::PlatformSpecific::SlesAudioEngine();
        
            SLEngineOption options[] = { SL_ENGINEOPTION_THREADSAFE, SL_BOOLEAN_TRUE };
            result = slCreateEngine(&newAudioEngine->EngineObject, 1, options, 0, NULL, NULL);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // realize the engine
            result = (*newAudioEngine->EngineObject)->Realize(newAudioEngine->EngineObject,SL_BOOLEAN_FALSE);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // get the engine interface, which is needed in order to create other objects
            result = (*newAudioEngine->EngineObject)->GetInterface(newAudioEngine->EngineObject, SL_IID_ENGINE, &newAudioEngine->EngineEngine);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // create output mix
            result = (*newAudioEngine->EngineEngine)->CreateOutputMix(newAudioEngine->EngineEngine, &newAudioEngine->OutputMixObject, 0, NULL, NULL);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;

            // realize the output mix
            // [TODO] What does that FALSE do?
            result = (*newAudioEngine->OutputMixObject)->Realize(newAudioEngine->OutputMixObject, SL_BOOLEAN_FALSE);
            assert(SL_RESULT_SUCCESS == result);
            (void)result;
            if (GlobalAAudioEngine != 0) GlobalAAudioEngine->Release();
            GlobalAAudioEngine = newAudioEngine;        
        
            LOGD("Engine Created");
        }

        static void Shutdown()
        {
            // destroy engine object and interfaces
            if (GlobalAAudioEngine->EngineObject != NULL) {
                (*GlobalAAudioEngine->EngineObject)->Destroy(GlobalAAudioEngine->EngineObject);
                GlobalAAudioEngine->EngineObject = NULL;
                GlobalAAudioEngine->EngineEngine = NULL;
            }
        }
    };

	static int SoundRefCount = 0;
    static void AssertInit()
	{
		if (!SoundRefCount)
		{
            SimpleAudioEngine::Startup();
			atexit(SimpleAudioEngine::Shutdown);
		}
	}

    SimpleSound* SimpleSound::Create(const String& path, bool asset)
    {
        AssertInit();
        return new SlesSimpleSound(path, asset);
    }
}
