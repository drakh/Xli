#include "SlesAudioBuffer.h"
#include <XliAudio/SimpleAudio.h>
#include "../../core/android/AXliJ.h"

extern Xli::SlesAudioEngine* GlobalAAudioEngine;

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
        
    public:
        SlesSimpleSoundChannel(const SimpleSound& sound)
        {
            playerObject = NULL;
            playerPlayItf = NULL;
            playerSeekItf = NULL;
            playerVolumeItf = NULL;
            Closed = !PrepareSlesPlayer(sound.Path, sound.Asset);            
        }    

        virtual ~SlesSimpleSoundChannel()
        {
            playerPlayItf = NULL;
            playerSeekItf = NULL;
            playerVolumeItf = NULL;
            (*playerObject)->Destroy(playerObject);
            Closed=true;
        }

        virtual int GetDuration() const
        {
            SLmillisecond duration = -1;
            SLresult result = (*playerPlayItf)->GetPosition(playerPlayItf, &duration);
            assert(SL_RESULT_SUCCESS == result);
            return (int)duration;
        }
        
        virtual void Play()
        {
            (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PLAYING);
        }

        virtual void SetLoop(double milli_start, double milli_end)
        {                     
            SLresult result = (*playerSeekItf)->SetLoop(playerSeekItf, SL_BOOLEAN_TRUE,
                                                        (SLmillisecond)milli_start,
                                                        (SLmillisecond)milli_end);
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
        
        virtual int GetPosition() const
        {
            SLmillisecond pos = -1;
            SLresult result = (*playerPlayItf)->GetPosition(playerPlayItf, &pos);
            assert(SL_RESULT_SUCCESS == result);
            return (int)pos;
        }

        virtual void SetPosition(int position)
        {
            SLresult result = (*playerSeekItf)->SetPosition(playerSeekItf, position, SL_SEEKMODE_FAST);
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
        
		virtual float GetVolume() const
        {
            SLmillibel max;
            (*playerVolumeItf)->GetMaxVolumeLevel(playerVolumeItf, &max);
            SLmillibel vol;
            SLresult result = (*playerVolumeItf)->GetVolumeLevel(playerVolumeItf, &vol);
            assert(SL_RESULT_SUCCESS == result);
            return (100.0f / (max - SL_MILLIBEL_MIN)) * vol;
        }

		virtual void SetVolume(float volume) const
        {
            SLmillibel max;
            (*playerVolumeItf)->GetMaxVolumeLevel(playerVolumeItf, &max);
            SLmillibel vol = SL_MILLIBEL_MIN + (((max - SL_MILLIBEL_MIN) / 100.0f) * volume);
            if (vol > max) vol = max;
            if (vol < SL_MILLIBEL_MIN) vol = SL_MILLIBEL_MIN;
            LOGDBG("min: %d max: %d vol-in: %f vol: %d", SL_MILLIBEL_MIN, max, volume, vol);
            SLresult result = (*playerVolumeItf)->SetVolumeLevel(playerVolumeItf, vol);
            assert(SL_RESULT_SUCCESS == result);
        }

		virtual void Pause()
        {
            (*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_PAUSED);
        }

		virtual bool IsPaused()
        {
            SLuint32 pState;
            SLresult result = (*playerPlayItf)->GetPlayState(playerPlayItf, &pState);
            assert(SL_RESULT_SUCCESS == result);
            return (pState == SL_PLAYSTATE_PAUSED);
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

		virtual void Stop()
        {
            SLresult result=(*playerPlayItf)->SetPlayState(playerPlayItf, SL_PLAYSTATE_STOPPED);
            assert(SL_RESULT_SUCCESS == result);
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

                AAssetManager* mgr = XliJ::GetAssetManager();
                assert(NULL != mgr);
                AAsset* assetp = AAssetManager_open(mgr, path.Data(), AASSET_MODE_UNKNOWN);
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

                SLDataLocator_URI locatorIn = {SL_DATALOCATOR_URI, (SLchar *)path.Data()};
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

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    class SlesSimpleSound : public SimpleSound
    {
    private:
        double duration;
    public:
        SlesSimpleSound(const String& path, bool androidAsset)
        {
            duration = -1;
            this->Path = path;
            this->Asset = androidAsset;
            PopulateMetadata();
        }

        virtual double GetDuration()
        {
            return duration;
        }
        
        virtual void PopulateMetadata()
        {
            SlesSimpleSoundChannel* channel = new SlesSimpleSoundChannel(sound);
            duration = channel.GetDuration();
            delete(result);
        }
    };
	SimpleSound* SimpleSound::LoadFromAssets(const String& path)
	{
        if (!GlobalAAudioEngine) AudioBuffer::Init();
		return new SlesSimpleSound(path, true);
	}
	SimpleSound* SimpleSound::LoadFromFS(const String& path)
	{
        if (!GlobalAAudioEngine) AudioBuffer::Init();
		return new SlesSimpleSound(path, false);
	}

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    class SlesSimpleSoundPlayer : public SimpleSoundPlayer
    {
    public:        
        SlesSimpleSoundPlayer()
        {   
            if (!GlobalAAudioEngine) AudioBuffer::Init();
        }

        virtual SimpleSoundChannel* Play(const SimpleSound& sound, bool paused)
        {
            SlesSimpleSoundChannel* result = new SlesSimpleSoundChannel(sound);
            if (!paused) result->Play();
            return (SimpleSoundChannel*)result;
        }

		virtual SimpleSoundChannel* PlayLoop(const SimpleSound& sound, bool paused)
        {
            SlesSimpleSoundChannel* result = new SlesSimpleSoundChannel(sound);
            if (paused) result->Pause();
            // [TODO] set looping
            return (SimpleSoundChannel*)result;
        }
    };
    SimpleSoundPlayer* SimpleSoundPlayer::Create()
    {
        return new SlesSimpleSoundPlayer();
    }
}
