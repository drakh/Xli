#include <Xli/MessageBox.h>
#include "SlesAudioBuffer.h"

extern Xli::SlesAudioEngine* GlobalAAudioEngine = 0;

namespace Xli
{
    void SlesAudioBuffer::Startup()
    {
        SLresult result;
        SlesAudioEngine* newAudioEngine = new SlesAudioEngine();
        
        SLEngineOption options[] = { SL_ENGINEOPTION_THREADSAFE, SL_BOOLEAN_TRUE };
        result = slCreateEngine(&newAudioEngine->EngineObject, 1, options, 0, NULL, NULL);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // realize the engine
        result = (*newAudioEngine->EngineObject)->Realize(newAudioEngine->EngineObject, 
                                                          SL_BOOLEAN_FALSE);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // get the engine interface, which is needed in order to create other objects
        result = (*newAudioEngine->EngineObject)->GetInterface(newAudioEngine->EngineObject, 
                                                               SL_IID_ENGINE, 
                                                               &newAudioEngine->EngineEngine);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // create output mix
        result = (*newAudioEngine->EngineEngine)->CreateOutputMix(newAudioEngine->EngineEngine, &newAudioEngine->OutputMixObject, 0, NULL, NULL);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // realize the output mix
        // [TODO] What does that FALSE do?
        result = (*newAudioEngine->OutputMixObject)->Realize(newAudioEngine->OutputMixObject, 
                                                             SL_BOOLEAN_FALSE);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
        if (GlobalAAudioEngine != 0) GlobalAAudioEngine->Release();
        GlobalAAudioEngine = newAudioEngine;        
        
        LOGDBG("Engine Created");
    }

    void SlesAudioBuffer::Shutdown()
    {
        // destroy engine object and interfaces
        if (GlobalAAudioEngine->EngineObject != NULL) {
            (*GlobalAAudioEngine->EngineObject)->Destroy(GlobalAAudioEngine->EngineObject);
            GlobalAAudioEngine->EngineObject = NULL;
            GlobalAAudioEngine->EngineEngine = NULL;
        }
    }

    void SlesAudioBuffer::CreateBufferQueueAudioPlayer()
    {
        SLresult result;
        
        // configure audio source
        // THe array members are the locatorType and the number of buffers
        SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
        //formatType, numChannels, samplesPerSec, bitsPerSample, containerSize, channelMask, endianness
        SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_16,
                                       SL_PCMSAMPLEFORMAT_FIXED_16, 
                                       SL_PCMSAMPLEFORMAT_FIXED_16,
                                       SL_SPEAKER_FRONT_CENTER,
                                       SL_BYTEORDER_LITTLEENDIAN};
        SLDataSource audioSrc = {&loc_bufq, &format_pcm};

        // configure audio sink
        // locatorType, outputMix
        SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, 
                                              GlobalAAudioEngine->OutputMixObject};
        // pLocator, pFormat
        SLDataSink audioSnk = {&loc_outmix, NULL};

        // create audio player
        // We say we require these interfaces, after realization we can use GetInterface
        // to bring back the interfaces. (see below)
        const SLInterfaceID ids[3] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
        const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
        result = (*GlobalAAudioEngine->EngineEngine)->CreateAudioPlayer(GlobalAAudioEngine->EngineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 3, ids, req);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // realize the player
        result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // get the play interface
        result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // get the buffer queue interface
        result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &BQPlayerBufferQueue);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // register callback on the buffer queue
        result = (*BQPlayerBufferQueue)->RegisterCallback(BQPlayerBufferQueue,
                                                          BufferQueueCallback, this);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;

        // get the volume interface
        result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, 
                                                 &bqPlayerVolume);
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
        LOGDBG("Created BufferQueue Player");
    }

    // [TODO]
    void SlesAudioBuffer::BufferQueueCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
    {
        SLresult result;

        SlesAudioBuffer* audioBuffer = (SlesAudioBuffer*)context;
        int bufferSize = audioBuffer->BufferSize;
        unsigned char* byteBuffer = audioBuffer->ByteBuffer;

        int bytesRead = audioBuffer->src->Read(byteBuffer, 1, bufferSize);
        if (bytesRead > 0) {
            result = (*audioBuffer->BQPlayerBufferQueue)->Enqueue(audioBuffer->BQPlayerBufferQueue, byteBuffer, bytesRead);
        }
        // [TODO] What other posibilities and how do we handle them?
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
    }

    void SlesAudioBuffer::EchoStats() const
    {
        LOGDBG("Channels: %d", this->channels);
        LOGDBG("SampleRate: %f", this->sampleRate);
        LOGDBG("BufferSize: %d", this->BufferSize);
    }

    // [TODO] Fix all these values
    SlesAudioBuffer::SlesAudioBuffer(Stream* source, DataType dataType, int channels,
                                     double sampleRate, int latency, int framesPerBuffer)
    {
        this->bqPlayerObject = NULL;
        this->src = source;
        this->channels = channels;
        this->dataType = dataType;
        this->sampleRate = 44100; //sampleRate; [TODO] HACK TO PROVE CONCEPT---FIXME
        this->BufferSize = framesPerBuffer;
        this->ByteBuffer = (unsigned char*)malloc(this->BufferSize);
        CreateBufferQueueAudioPlayer();
        EchoStats();
    }
    SlesAudioBuffer::SlesAudioBuffer(Stream* source, DataType dataType, int channels,
                                     double sampleRate)
    {
        this->bqPlayerObject = NULL;
        this->src = source;
        this->channels = channels;
        this->dataType = dataType;
        this->sampleRate = 44100; //sampleRate; [TODO] HACK TO PROVE CONCEPT---FIXME
        this->BufferSize = 512;
        this->ByteBuffer = (unsigned char*)malloc(this->BufferSize);
        CreateBufferQueueAudioPlayer();
        EchoStats();
    }

    SlesAudioBuffer::~SlesAudioBuffer()
    {
        this->src->Close();
        if (bqPlayerObject != NULL) 
        {
            (*bqPlayerObject)->Destroy(bqPlayerObject);
            bqPlayerObject = NULL;
            bqPlayerPlay = NULL;
            BQPlayerBufferQueue = NULL;
            bqPlayerMuteSolo = NULL;
            bqPlayerVolume = NULL;
        }
        free(this->ByteBuffer);
    }

    void SlesAudioBuffer::Play()
    {
        SLresult result;
        result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
        if (result == SL_RESULT_SUCCESS)
        {
            BufferQueueCallback(BQPlayerBufferQueue, this);
        } else {
            XLI_THROW("Unable to start sound stream: [TODO] This needs details");
        }
        (void)result;
    }

    void SlesAudioBuffer::Stop()
    {
        SLresult result;
        result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_STOPPED);
        if (result != SL_RESULT_SUCCESS)
        {
            XLI_THROW("Unable to start sound stream: [TODO] This needs details");
        }
        (void)result;
    }

    DataType SlesAudioBuffer::GetDataType() const
    {
        return dataType;
    }

    int SlesAudioBuffer::GetChannelCount() const
    {
        return channels;
    }

    double SlesAudioBuffer::GetSampleRate() const
    {
        return sampleRate;
    }

    // [TODO] Implement GetLatency
    double SlesAudioBuffer::GetLatency() const
    {
        XLI_THROW("[TODO] Implement GetLatency...somehow");
        return 0;
    }

//------------------------------------

	static int SoundRefCount = 0;
	void AudioBuffer::Init()
	{
		if (SoundRefCount == 0)
		{
            LOGDBG("Starting to Initialise Audio");
            SlesAudioBuffer::Startup();
            LOGDBG("Finished Initialising Audio");
		}
		SoundRefCount++;
	}

	void AudioBuffer::Done()
    {
		SoundRefCount--;
		if (SoundRefCount == 0)
		{
            SlesAudioBuffer::Shutdown();
		}
		if (SoundRefCount < 0)
		{
			XLI_THROW_BAD_DELETE;
		}
	}

	static void AssertInit()
	{
		if (!SoundRefCount)
		{
			AudioBuffer::Init();
			atexit(AudioBuffer::Done);
		}
	}

	AudioBuffer* AudioBuffer::Create(Stream* source, DataType dataType, int channels, double sampleRate, int latency, int framesPerBuffer)
	{
		AssertInit();
		return new SlesAudioBuffer(source, dataType, channels, sampleRate, latency, framesPerBuffer);
	}

	AudioBuffer* AudioBuffer::Create(Stream* source, DataType dataType, int channels, double sampleRate)
	{
		AssertInit();
		return new SlesAudioBuffer(source, dataType, channels, sampleRate);
	}

	AudioBuffer* AudioBuffer::Create(AudioStream* source)
	{
		AssertInit();
		return Create(source, source->GetDataType(), source->GetChannelCount(),
                      source->GetSampleRate());
	}
}
