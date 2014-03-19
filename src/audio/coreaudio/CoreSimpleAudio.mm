#include <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
#include <Xli/PlatformSpecific/iOS.h>
#include <XliAudio/SimpleAudio.h>
#include <Xli/Array.h>
#include <Xli/Console.h>
#include <assert.h>

namespace Xli
{      

    class CoreAudioChannel : public SimpleSoundChannel
    {
        AVAudioPlayer* player;
            
    public:
        CoreAudioChannel(const char* filename, bool loop, bool play)
        {
            NSString* string = [NSString stringWithFormat:@"%@/data/%@", [[NSBundle mainBundle] resourcePath], [NSString stringWithUTF8String:filename]];
                
            NSURL* url = [NSURL fileURLWithPath:string];
            //[string release];
                
            NSError* error;
            
            BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:string];
            
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
        
    class CoreSimpleSound : public SimpleSound 
    {
    private:
        double duration;
        bool isasset;
        String path;
    public:
        CoreSimpleSound(const String& path, bool asset)
        {
            this->path = path;
            this->isasset = asset;
            
            CoreAudioChannel* c = new CoreAudioChannel(path.DataPtr(), false, false);
            this->duration = c->GetDuration();
            delete c;            
        }
        virtual double GetDuration() const
        {
            return duration;
        }
        virtual SimpleSoundChannel* Play(bool paused)
        {
            SimpleSoundChannel* result;
            result = new CoreAudioChannel(path.DataPtr(), false, true);
            return result;
        }
        virtual SimpleSoundChannel* PlayLoop(bool paused)
        {
            SimpleSoundChannel* result;
            result = new CoreAudioChannel(path.DataPtr(), true, true);
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
