#include <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
#include <Xli/PlatformSpecific/iOS.h>
#include <XliAudio/SimpleAudio.h>
#include <Xli/Array.h>
#include <Xli/Console.h>
#include <assert.h>

namespace Xli
{
    class AudioPacketBuffer
    {
        Array<unsigned long> buffersSizeBytes;
        Array<Byte*> buffers;
        Array<unsigned long> bufferPacketNums;
        Array<AudioStreamPacketDescription*> bufferPacketDescs;
            
    public:
        AudioStreamBasicDescription DataFormat;
        unsigned long BufferSizeBytes;
        unsigned long NumPacketsToRead;
        unsigned long CookieSize;
        char* Cookie;
        unsigned long AudioChannelLayoutSize;
        AudioChannelLayout* AudioChannelLayout;
            
        AudioPacketBuffer(const char* filename)
        {
            NSString* string = [NSString stringWithFormat:@"%@/data/%@", [[NSBundle mainBundle] resourcePath], [NSString stringWithUTF8String:filename]];
                
            NSURL* url = [NSURL fileURLWithPath:string];
            //[string release];
                
            AudioFileID file;
            if (AudioFileOpenURL((CFURLRef)url, kAudioFileReadPermission, 0, &file) != noErr)
            {
                ErrorPrintLine("ERROR: Failed to open audio file.");
                //[url release];
                return;
            }
            //[url release];
                
            unsigned long size = sizeof(DataFormat);
            if (AudioFileGetProperty(file, kAudioFilePropertyDataFormat, &size, &DataFormat) != noErr)
            {
                ErrorPrintLine("ERROR: Failed to get audio file properties.");
                return;
            }
                
            unsigned long maxPacketSize;
            size = sizeof(maxPacketSize);
            if (AudioFileGetProperty(file, kAudioFilePropertyPacketSizeUpperBound, &size, &maxPacketSize) != noErr)
            {
                Xli::ErrorPrintLine("ERROR: Failed to read max packet size.");
                return;
            }
                
            if (DataFormat.mFramesPerPacket)
            {
                Float64 numPacketsForTime = DataFormat.mSampleRate / DataFormat.mFramesPerPacket * .5; // Half-second buffers
                BufferSizeBytes = numPacketsForTime * maxPacketSize;
            }
            else
            {
                BufferSizeBytes = maxPacketSize;
            }
            NumPacketsToRead = BufferSizeBytes / maxPacketSize;
                
            CookieSize = sizeof(unsigned long);
            OSStatus result = AudioFileGetPropertyInfo(file, kAudioFilePropertyMagicCookieData, &CookieSize, NULL);
            if (!result && CookieSize)
            {
                Cookie = new char[CookieSize];
                if (AudioFileGetProperty(file, kAudioFilePropertyMagicCookieData, &CookieSize, Cookie) != noErr)
                {
                    Xli::ErrorPrintLine("ERROR: Failed to read cookie data.");
                    return;
                }
            }
            else
            {
                CookieSize = 0;
                Cookie = 0;
            }
                
            result = AudioFileGetProperty(file, kAudioFilePropertyChannelLayout, &AudioChannelLayoutSize, NULL);
            if (result == noErr && size > 0)
            {
                AudioChannelLayout = new ::AudioChannelLayout();
                if (AudioFileGetProperty(file, kAudioFilePropertyChannelLayout, &AudioChannelLayoutSize, AudioChannelLayout) != noErr)
                {
                    Xli::ErrorPrintLine("ERROR: Failed to read channel layout.");
                    return;
                }
            }
            else
            {
                AudioChannelLayoutSize = 0;
                AudioChannelLayout = 0;
            }
                
            Byte* tempBuffer = new Byte[BufferSizeBytes];
            AudioStreamPacketDescription* tempPacketDescs = new AudioStreamPacketDescription[NumPacketsToRead];
            SInt64 currentPacket = 0;
            while (true)
            {
                unsigned long numBytes;
                unsigned long numPackets = NumPacketsToRead;
                if (AudioFileReadPackets(file, false, &numBytes, tempPacketDescs, currentPacket, &numPackets, tempBuffer) != noErr)
                {
                    Xli::ErrorPrintLine("ERROR: Failed to read audio packets.");
                    delete tempBuffer;
                    delete tempPacketDescs;
                    return;
                }
                if (!numPackets) break;
                buffersSizeBytes.Add(numBytes);
                Byte* bufferData = new Byte[numBytes];
                for (int i = 0; i < numBytes; i++) bufferData[i] = tempBuffer[i];
                buffers.Add(bufferData);
                bufferPacketNums.Add(numPackets);
                AudioStreamPacketDescription* descData = new AudioStreamPacketDescription[numPackets];
                for (int i = 0; i < numPackets; i++) descData[i] = tempPacketDescs[i];
                bufferPacketDescs.Add(descData);
                currentPacket += numPackets;
            }
            delete tempBuffer;
            delete tempPacketDescs;
                
            AudioFileClose(file);
        }
            
        ~AudioPacketBuffer()
        {
            for (int i = 0; i < buffers.Length(); i++) delete buffers[i];
            for (int i = 0; i < bufferPacketDescs.Length(); i++) delete bufferPacketDescs[i];
            if (Cookie) delete Cookie;
            if (AudioChannelLayout) delete AudioChannelLayout;
        }
            
        void ReadBuffer(unsigned long bufferIndex, unsigned long* numBytes, AudioStreamPacketDescription* descs, unsigned long* numPackets, void* output)
        {
            if (bufferIndex >= buffers.Length())
            {
                *numBytes = *numPackets = 0;
                return;
            }
                
            *numBytes = buffersSizeBytes[bufferIndex];
            *numPackets = bufferPacketNums[bufferIndex];
            for (int i = 0; i < *numPackets; i++) descs[i] = bufferPacketDescs[bufferIndex][i];
            for (int i = 0; i < *numBytes; i++) ((Byte*)output)[i] = buffers[bufferIndex][i];
        }
    };
        
    class AudioQueueChannel : public SimpleSoundChannel
    {
        const static int numBuffers = 3;
            
        AudioPacketBuffer* packetBuffer;
        AudioQueueRef queue;
        AudioQueueBufferRef buffers[numBuffers];
        bool playing;
        bool finished;
        bool looping;
        unsigned long currentBuffer;
            
        static void bufferCallback(void* userData, AudioQueueRef aq, AudioQueueBufferRef aqb)
        {
                
                
            AudioQueueChannel* c = (AudioQueueChannel*)userData;
                
            if (!c->playing || c->finished) return;
                
            c->packetBuffer->ReadBuffer(c->currentBuffer, &(aqb->mAudioDataByteSize), aqb->mPacketDescriptions, &(aqb->mPacketDescriptionCount), aqb->mAudioData);
            if (aqb->mPacketDescriptionCount > 0)
            {
                if (AudioQueueEnqueueBuffer(aq, aqb, 0, NULL) != noErr) Xli::ErrorPrintLine("ERROR: Failed to enqueue audio buffer.");
                c->currentBuffer++;
            }
            else
            {
                if (c->looping)
                {
                    c->currentBuffer = 0;
                    bufferCallback(userData, aq, aqb);
                }
                else
                {
                    c->finished = true;
                }
            }
        }
            
    public:
        AudioQueueChannel(AudioPacketBuffer* packetBuffer, bool loop)
        {
            this->packetBuffer = packetBuffer;
                
            if (AudioQueueNewOutput(&packetBuffer->DataFormat, bufferCallback, this, CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &queue) != noErr)
            {
                Xli::ErrorPrintLine("ERROR: Failed to create output queue.");
                return;
            }
                
            unsigned long barf = kAudioQueueHardwareCodecPolicy_PreferSoftware;
            AudioQueueSetProperty(queue, kAudioQueueProperty_HardwareCodecPolicy, &barf, sizeof(barf));
                
            if (packetBuffer->CookieSize)
            {
                if (AudioQueueSetProperty(queue, kAudioQueueProperty_MagicCookie, packetBuffer->Cookie, packetBuffer->CookieSize) != noErr)
                {
                    Xli::ErrorPrintLine("ERROR: Failed to set cookie data on queue.");
                    return;
                }
            }
                
            if (packetBuffer->AudioChannelLayoutSize)
            {
                if (AudioQueueSetProperty(queue, kAudioQueueProperty_MagicCookie, &packetBuffer->AudioChannelLayout, packetBuffer->AudioChannelLayoutSize) != noErr)
                {
                    Xli::ErrorPrintLine("ERROR: Failed to set channel layout on queue.");
                    return;
                }
            }
                
            bool isVbr = (packetBuffer->DataFormat.mBytesPerPacket == 0 || packetBuffer->DataFormat.mFramesPerPacket == 0);
            for (int i = 0; i < numBuffers; i++)
            {
                if (AudioQueueAllocateBufferWithPacketDescriptions(queue, packetBuffer->BufferSizeBytes, isVbr ? packetBuffer->NumPacketsToRead : 0, &buffers[i]) != noErr)
                {
                    Xli::ErrorPrintLine("ERROR: Failed to allocate buffer.");
                    return;
                }
            }
                
            AudioQueueSetParameter(queue, kAudioQueueParam_Volume, 1.0);
                
            this->looping = loop;
            playing = true;
            finished = false;
            currentBuffer = 0;
            for (int i = 0; i < numBuffers; i++) bufferCallback(this, queue, buffers[i]);
            OSStatus err = AudioQueueStart(queue, NULL);
            if (err != noErr)
            {
                Xli::ErrorPrintLine("ERROR: Failed to start playback.");
                return;
            }
        }
            
        virtual ~AudioQueueChannel()
        {
            if (playing) AudioQueueStop(queue, true);
            AudioQueueDispose(queue, true);
        }
        
        virtual void Pause()
        {
            //{TODO}
        }
        virtual void UnPause()
        {
            // TODO
        }
        
        virtual int GetPosition() const //{TODO} should be double
        {
            AudioTimeStamp timeStamp;
            AudioQueueGetCurrentTime(queue, NULL, &timeStamp, NULL);
            return (int)(timeStamp.mSampleTime / packetBuffer->DataFormat.mSampleRate);
        }
        virtual void SetPosition(int value) //{TODO} should be double
        {
            // TODO
        }
        
        virtual float GetVolume() const
        {
            float res;
            AudioQueueGetParameter(queue, kAudioQueueParam_Volume, &res);
            return res;
        }
        virtual void SetVolume(float value) const
        {
            AudioQueueSetParameter(queue, kAudioQueueParam_Volume, value);
        }
        
        virtual bool IsPlaying()
        {
            return playing;
        }
        virtual bool IsFinished()
        {
            return finished;
        }
        
        virtual int GetDuration() const
        {
            //{TODO}
            return 0;
        }
        
        virtual float GetPan() const
        {
            float res;
            AudioQueueGetParameter(queue, kAudioQueueParam_Pan, &res);
            return res;
        }
        virtual void SetPan(float value) const
        {
            AudioQueueSetParameter(queue, kAudioQueueParam_Pan, value);
        }
        
        virtual void Play()
        {
            if (!playing)
            {
                playing = true;
                AudioQueueStart(queue, NULL); //{TODO} fix
            }
        }
        virtual void Stop()
        {
            if (playing)
            {
                playing = false;
                AudioQueueStop(queue, true);
            }
        }
        
        virtual bool IsPaused()
        {
            return false;
        }
    }; 

    class CoreAudioChannel : public SimpleSoundChannel
    {
        AVAudioPlayer* player;
            
    public:
        //CoreAudioChannel(const char* filename, PlayMode mode, bool loop, bool play)
        CoreAudioChannel(const char* filename, bool loop, bool play)
        {
            NSString* string = [NSString stringWithFormat:@"%@/%@", [[NSBundle mainBundle] resourcePath], [NSString stringWithUTF8String:filename]];
                
            NSURL* url = [NSURL fileURLWithPath:string];
            //[string release];
                
            NSError* error;
            
            BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:string];
            //NSArray * directoryContents = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:[[NSBundle mainBundle] resourcePath] error:&error];
            
            if (!fileExists) {
                Xli::ErrorPrintLine("audio file not found");
                return;
            }
            
            player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
            //player = [[AVAudioPlayer alloc] initWithData:[NSData dataWithContentsOfURL:url] error:&error];
            //[url release];
            
            if (player == nil)
            {
                Xli::ErrorPrintLine("ERROR: Failed to create AVAudioPlayer: " + (String)([[error description] UTF8String]));
                return;
            }
                
            if (loop)
            {
                player.numberOfLoops = -1;
            }
                
            if (play)
            {
                [player play];
            }
        }
            
        virtual ~CoreAudioChannel()
        {
            if (player == nil) return;
            [player release];
        }
        
        virtual void Pause()
        {
            if (player == nil) return;
            [player pause];
        }
        virtual void UnPause()
        {
            if (player == nil) return;
            [player play];
        }
        
        virtual int GetPosition() const // this should be double
        {
            if (player == nil) return 0.0;
            return (int)player.currentTime;
        }
        virtual void SetPosition(int value) // this shouldbe double, fix the headers
        {
            if (player == nil) return;
            player.currentTime = (double)value;
        }

        virtual float GetVolume() const
        {
            if (player == nil) return 0.0f;
            return player.volume;
        }
        virtual void SetVolume(float value) const
        {
            if (player == nil) return;
            player.volume = value;
        }
        virtual bool IsPlaying()
        {
            if (player == nil) return false;
            return player.playing;
        }
        virtual bool IsFinished()
        {
            if (player == nil) return true;
            return false;
        }
   
        int GetDuration() const // this should be double
        {
            if (player == nil) return 0.0;
            return (int)player.duration;
        }
        virtual float GetPan() const
        {
            if (player == nil) return 0.0f;
            return player.pan;
        }
        virtual void SetPan(float value) const
        {
            if (player == nil) return;
            player.pan = value;
        }
        
        virtual void Play()
        {
            if (player == nil) return;
            [player play];
        }
        virtual void Stop()
        {
            if (player == nil) return;
            [player stop];
        }
        
        virtual bool IsPaused()
        {
            if (player == nil) return true;
            return !player.playing;
        }
    };
        
    class DummyChannel : public SimpleSoundChannel
    {
    public:
        DummyChannel()
        {
        }
        
        virtual ~DummyChannel()
        {
        }
        
        virtual double GetPosition()
        {
            return 0.0;
        }
        virtual void SetPosition(double value)
        {
        }
        
        virtual float GetPan()
        {
            return 0.0f;
        }
        virtual void SetPan(float value)
        {
        }
        
        virtual float GetVolume()
        {
            return 0.0f;
        }
        virtual void SetVolume(float value)
        {
        }
        
        virtual bool GetPause()
        {
            return false;
        }
        virtual void SetPause(bool value)
        {
        }
        
        virtual bool GetIsPlaying()
        {
            return false;
        }
        virtual bool GetIsFinished()
        {
            return true;
        }
        
        virtual void Stop()
        {
        }
    };

            
    class CoreSimpleSound : public SimpleSound 
    {
    private:
        double duration;
        bool isasset;
        String path;
        AudioPacketBuffer* bufferedData;
    public:
        CoreSimpleSound(const String& path, bool asset)
        {
            this->path = path;
            //this->mode = ?????????????????????;
            this->isasset = asset;
            
            //CoreAudioChannel* c = new CoreAudioChannel(this->path.Data(), mode, false, false);
            CoreAudioChannel* c = new CoreAudioChannel(path.Data(), false, false);
            this->duration = c->GetDuration();
            delete c;
            
            //if (mode != PlayModeStream) {
            //bufferedData = new AudioPacketBuffer(path.Data());
            //} else {
            this->bufferedData = 0;
            //}
        }
        virtual double GetDuration() const
        {
            return duration;
        }
        virtual SimpleSoundChannel* Play(bool paused)
        {
            SimpleSoundChannel* result;
            //if (mode != PlayModeStream)
            //{
            //    result = new AudioQueueChannel((AudioPacketBuffer*)bufferedData, false);
            //} else {
            result = new CoreAudioChannel(path.Data(), false, true); // {TODO} originally took 'mode'
            //}
            //if (!paused) result->Play();
            return result;
        }
        virtual SimpleSoundChannel* PlayLoop(bool paused)
        {
            SimpleSoundChannel* result;
            //if (mode != PlayModeStream)
            //{
            //    result = new AudioQueueChannel((AudioPacketBuffer*)bufferedData, true);
            //} else {
            result = new CoreAudioChannel(path.Data(), true, true); // {TODO} originally took 'mode'
            //}
            return result;
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
    
    SimpleSound* SimpleSound::Create(const String& path, bool asset)
    {
        return new CoreSimpleSound(path, asset);
    }
}
