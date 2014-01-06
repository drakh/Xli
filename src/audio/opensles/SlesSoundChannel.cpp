#include "Channel.h"

#include <Xli.h>
#include <XliPlatform.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

namespace Uno
{
    namespace Audio
    {
        // TODO: Please clean up this code.
        // TODO2: Implement buffer queues later (for better performance when playing sound effects)

        static SLObjectItf engineObject = 0;
        static SLEngineItf engineEngine = 0;
        static SLObjectItf outputMixObject = 0;

        static void ReleaseAudio()
        {
            // destroy output mix object, and invalidate all associated interfaces
            if (outputMixObject != 0) 
            {
                (*outputMixObject)->Destroy(outputMixObject);
                outputMixObject = 0;
            }

            // destroy engine object, and invalidate all associated interfaces
            if (engineObject != 0) 
            {
                (*engineObject)->Destroy(engineObject);
                engineObject = 0;
                engineEngine = 0;
            }
        }

        Channel::Channel(int fd, off_t start, off_t length)
            : fdPlayerObject(0)
        {
            //paused = false;

            //Xli::ErrorPrintLine("DEBUUG new");
            SLresult result;

            if (engineObject == 0)
            {
                result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to create OpenSL engine");

                result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to realize OpenSL engine");

                result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to get engine interface for OpenSL engine");

                //const SLInterfaceID ids[0] = {SL_IID_ENVIRONMENTALREVERB};
                //const SLboolean req[1] = {SL_BOOLEAN_FALSE};
                result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, 0, 0);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to create OpenSL output mix");

                result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to realize OpenSL output mix");

                atexit(ReleaseAudio);
            }

            try
            {
                // configure audio source
                SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
                SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
                SLDataSource audioSrc = {&loc_fd, &format_mime};

                // configure audio sink
                SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
                SLDataSink audioSnk = {&loc_outmix, NULL};

                // create audio player
                const SLInterfaceID ids[2] = {SL_IID_SEEK, SL_IID_VOLUME};
                const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

                result = (*engineEngine)->CreateAudioPlayer(engineEngine, &fdPlayerObject, &audioSrc, &audioSnk, 2, ids, req);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to create audio player (" + Xli::String::HexFromInt((int)result) + ")");

                result = (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to realize audio player (" + Xli::String::HexFromInt((int)result) + ")");

                result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY, &fdPlayerPlay);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to get play interface");
                result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_SEEK, &fdPlayerSeek);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to get seek interface");
                result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_VOLUME, &fdPlayerVolume);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to get volume control interface");

                /*
                SLmillibel maxvol;
                result = (*fdPlayerVolume)->GetMaxVolumeLevel(fdPlayerVolume, &maxvol);
                if (SL_RESULT_SUCCESS != result) XLI_THROW("OPENSL ERROR: Failed to get max volume level");
                */

                result = (*fdPlayerVolume)->EnableStereoPosition(fdPlayerVolume, true); 
                if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("OPENSL ERROR: Failed to enable stereo positioning");
            }
            catch (const Xli::Exception& e)
            {
                Xli::ErrorPrintLine(e.GetMessage());
                if (fdPlayerObject != 0) (*fdPlayerObject)->Destroy(fdPlayerObject);
                fdPlayerObject = 0;
            }
        }

        Channel::~Channel()
        {
            Stop();
        }

        double Channel::GetDuration()
        {
            //Xli::PrintLine("GetDuration");

            if (fdPlayerObject == 0) return 0.0;

            SLmillisecond ms;
            (*fdPlayerPlay)->GetDuration(fdPlayerPlay, &ms);
            return double(ms) * 0.001;
        }

        double Channel::GetPosition()
        {
            //Xli::PrintLine("GetPosition");

            if (fdPlayerObject == 0) return 0.0;

            SLmillisecond ms;
            (*fdPlayerPlay)->GetPosition(fdPlayerPlay, &ms);
            return double(ms) * 0.001;
        }

        void Channel::SetPosition(double position)
        {
            //Xli::PrintLine("SetPosition");

            if (fdPlayerObject == 0) return;

            SLmillisecond ms = SLmillisecond(position * 1000.0);
            (*fdPlayerSeek)->SetPosition(fdPlayerSeek,  ms, SL_SEEKMODE_FAST);
        }

        const float MillibelFactor = 2000.0f;

        static SLmillibel FromLinear(float f)
        {
            if (f <= 0.0f) return SL_MILLIBEL_MIN;
            if (f > 1.0f) f = 1.0f;

            float b = MillibelFactor * log10f(f);

            //Xli::ErrorPrintLine("vmax: " + (Xli::CharString)vmax);

            return (SLmillibel)b;
        }

        static float ToLinear(SLmillibel m)
        {
            float b = (float)m;
            float v = powf(10.0f, b / MillibelFactor);

            return v;
        }

        float Channel::GetVolume()
        {
            //Xli::PrintLine("GetVolume");

            if (fdPlayerObject == 0) return 0.0f;

            SLresult result;
            SLmillibel vol;
            result = (*fdPlayerVolume)->GetVolumeLevel(fdPlayerVolume, &vol);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Channel::GetVolume() failed");

            return ToLinear(vol);
        }
    
        void Channel::SetVolume(float volume)
        {
            //Xli::PrintLine("SetVolume");
            
            //return;

            if (fdPlayerObject == 0) return;

            SLresult result;
            result = (*fdPlayerVolume)->SetVolumeLevel(fdPlayerVolume, FromLinear(volume));
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Channel::SetVolume() failed");
        }

        float Channel::GetPan()
        {
            //Xli::PrintLine("GetPan");

            if (fdPlayerObject == 0) { Xli::ErrorPrintLine("OPENSL ERROR: Audio player was destroyed"); return 0.0f; }

            SLresult result;
            SLpermille pos;
            result = (*fdPlayerVolume)->GetStereoPosition(fdPlayerVolume, &pos);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Channel::GetPan() failed");
            return double(pos)*0.001;
        }

        void Channel::SetPan(float position)
        {
            //Xli::PrintLine("SetPan");

            if (fdPlayerObject == 0) return;

            SLresult result;
            SLpermille pos = SLpermille(Xli::Clamp(position, -1.0f, 1.0f)*1000.0f);
            result = (*fdPlayerVolume)->SetStereoPosition(fdPlayerVolume, pos);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Channel::SetPan() failed");
        }

        bool Channel::GetPaused()
        {
            //Xli::PrintLine("GetPaused");

            if (fdPlayerObject == 0) return false;

            SLresult result;
            SLuint32 state;
            result = (*fdPlayerPlay)->GetPlayState(fdPlayerPlay, &state);
            if (SL_RESULT_SUCCESS != result) { Xli::ErrorPrintLine("Audio::Channel::GetPaused() failed"); return false; }
            return state == SL_PLAYSTATE_PAUSED;
        }

        void Channel::SetPaused(bool value)
        {
            //Xli::PrintLine("SetPaused");

            //this->paused = value;
            if (fdPlayerObject == 0) return;

            SLresult result;
            result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, value ? SL_PLAYSTATE_PAUSED : SL_PLAYSTATE_PLAYING);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Channel::SetPaused() failed");

            SLuint32 state;
            result = (*fdPlayerPlay)->GetPlayState(fdPlayerPlay, &state);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Sound::SetPaused() failed");
            if (!value && state != SL_PLAYSTATE_PLAYING) Xli::ErrorPrintLine("Audio::Sound::SetPaused() failed - Sound is not playing");
            if (value && state != SL_PLAYSTATE_PAUSED) Xli::ErrorPrintLine("Audio::Sound::SetPaused() failed - Sound is still playing");
        }

        bool Channel::IsPlaying()
        {
            //Xli::PrintLine("IsPlaying");

            if (fdPlayerObject == 0) { return false; }

            SLresult result;
            SLuint32 state;
            result = (*fdPlayerPlay)->GetPlayState(fdPlayerPlay, &state);
            if (SL_RESULT_SUCCESS != result) { Xli::ErrorPrintLine("Audio::Channel::IsPlaying() failed"); return false; }
            return state == SL_PLAYSTATE_PLAYING;
        }

        bool Channel::IsFinished()
        {
            //Xli::PrintLine("IsFinished");

            if (stopping)
            {
                return true;
            }

            if (fdPlayerObject == 0) { return true; }

            return false;
            /*
            SLresult result;
            SLuint32 state;
            result = (*fdPlayerPlay)->GetPlayState(fdPlayerPlay, &state);
            if (SL_RESULT_SUCCESS != result) { Xli::ErrorPrintLine("Audio::Channel::IsFinished() failed"); return true; }

            if (state == SL_PLAYSTATE_STOPPED)
            {
                return true;
            }

            if (state == SL_PLAYSTATE_PAUSED && !paused)
            {
                SLmillisecond msP, msD;
                (*fdPlayerPlay)->GetPosition(fdPlayerPlay, &msD);
                (*fdPlayerPlay)->GetDuration(fdPlayerPlay, &msP);

                Xli::ErrorPrintLine("OPENSL WARNING: Channel paused but not paused by user - freeing channel, " + (Xli::CharString)(int)msP + ", " + (Xli::CharString)(int)msD);
                return true;
            }

            // XXX: this is a bit hackish, but sles gives me paused, not stopped here. a better solution would be to hook ourself on a callback for stopped playback or something.
            SLmillisecond msP, msD;
            (*fdPlayerPlay)->GetPosition(fdPlayerPlay, &msD);
            (*fdPlayerPlay)->GetDuration(fdPlayerPlay, &msP);

            return (msP == msD);
            */
        }

        void Channel::Stop()
        {
            //Xli::PrintLine("Stop");

            if (fdPlayerObject != 0)
            {
                //Xli::ErrorPrintLine("STOP CALLED");

                //(*fdPlayerPlay)->Destroy(fdPlayerPlay);
                //(*fdPlayerSeek)->Destroy(fdPlayerSeek);
                //(*fdPlayerVolume)->Destroy(fdPlayerVolume);
                /*
                SLresult result;
                result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_STOPPED);
                if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Channel::Stop() failed");
                Xli::PrintLine("Stopped");
                */

                // TODO: Sometimes this takes really long time

                (*fdPlayerObject)->Destroy(fdPlayerObject);
                fdPlayerObject = 0;

                //Xli::PrintLine("Stop Complete");
            }
        }

        void Channel::PlayCallback(SLPlayItf caller, void* context, SLuint32 event)
        {
            Channel* c = (Channel*)context;

            //Xli::ErrorPrintLine("Callback recieved " + Xli::String::HexFromInt((int)event));

            // destroy player when sound is finished
            if ((event & SL_PLAYEVENT_HEADATEND) == SL_PLAYEVENT_HEADATEND)
            {
                //Xli::ErrorPrintLine("Destroying player");
                c->stopping = true;
                /*
                (*(c->fdPlayerObject))->Destroy(c->fdPlayerObject);
                c->fdPlayerObject = 0;
                */
            }
        }

        void Channel::Play()
        {
            //Xli::PrintLine("Play");

            if (fdPlayerObject == 0) { Xli::ErrorPrintLine("OPENSL ERROR: Audio player was destroyed"); return; }

            SLresult result;
            result = (*fdPlayerPlay)->RegisterCallback(fdPlayerPlay, PlayCallback, (void*)this);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("OPENSL ERROR: Failed to register callback");
            result = (*fdPlayerPlay)->SetCallbackEventsMask(fdPlayerPlay, SL_PLAYEVENT_HEADATEND);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("OPENSL ERROR: Failed to set event mask");
            result = (*fdPlayerSeek)->SetLoop(fdPlayerSeek, SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("OPENSL ERROR: Failed to set looping mode");
            result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("OPENSL ERROR: Failed to set play state");

            SLuint32 state;
            result = (*fdPlayerPlay)->GetPlayState(fdPlayerPlay, &state);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("OPENSL ERROR: Failed to get play state");
            if (state != SL_PLAYSTATE_PLAYING) Xli::ErrorPrintLine("OPENSL ERROR: Failed - sound is not playing");

            stopping = false;
        }

        void Channel::PlayLoop()
        {
            //Xli::PrintLine("PlayLoop");

            if (fdPlayerObject == 0) { Xli::ErrorPrintLine("OPENSL ERROR: Audio player was destroyed"); return; }

            SLresult result;
            result = (*fdPlayerSeek)->SetLoop(fdPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Sound::PlayLoop() failed");
            result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Sound::PlayLoop() failed");
            
            SLuint32 state;
            result = (*fdPlayerPlay)->GetPlayState(fdPlayerPlay, &state);
            if (SL_RESULT_SUCCESS != result) Xli::ErrorPrintLine("Audio::Sound::PlayLoop() failed");
            if (state != SL_PLAYSTATE_PLAYING) Xli::ErrorPrintLine("Audio::Sound::PlayLoop() failed - Sound is not playing");

            stopping = false;
        }
    }
}
