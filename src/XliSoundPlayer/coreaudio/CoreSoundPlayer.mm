#include <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
#include <XliPlatform/PlatformSpecific/iOS.h>
#include <XliSoundPlayer/SoundPlayer.h>
#include <Xli/Array.h>
#include <Xli/Console.h>
#include <assert.h>

namespace Xli
{      

    class CoreSoundChannel : public SoundChannel
    {
        AVAudioPlayer* player;
            
    public:
        CoreSoundChannel(const char* filename, bool loop, bool play)
        {
            NSString* pathString = [NSString stringWithFormat:@"%@/data/%@", [[NSBundle mainBundle] resourcePath], [NSString stringWithUTF8String:filename]];
                
            NSURL* url = [NSURL fileURLWithPath:pathString];
            //[pathString release];
                
            NSError* error;
            
            BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:pathString];
            if (!fileExists) {
                const char* wrongPath = [pathString UTF8String];
                Error->WriteLine("SoundPlayer: Audio file not found at:");
                Error->WriteLine(wrongPath);
                player = nil;
                return;
            }
            
            player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
            //player = [[AVAudioPlayer alloc] initWithData:[NSData dataWithContentsOfURL:url] error:&error];
            //[url release];
            
            if (player == nil)
            {
                Error->WriteLine("ERROR: Failed to create AVAudioPlayer: " + (String)([[error description] UTF8String]));
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
            
        virtual ~CoreSoundChannel()
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
        
        virtual double GetPosition() const
        {
            if (player == nil) return 0.0;
            return player.currentTime;
        }
        virtual void SetPosition(double value)
        {
            if (player == nil) return;
            player.currentTime = value;
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
   
        double GetDuration() const
        {
            if (player == nil || player == 0) return 0;
            return (double)player.duration;
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
        
    class CoreSound : public Sound 
    {
    private:
        double duration;
        bool isasset;
        String path;
    public:
        CoreSound(const String& path, bool asset)
        {
            this->path = path;
            this->isasset = asset;
            
            CoreSoundChannel* c = new CoreSoundChannel(path.Ptr(), false, false);
            this->duration = c->GetDuration();
            delete c;
            this->duration = 0.0;
        }
        virtual double GetDuration() const
        {
            return duration;
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

    class CoreSoundPlayer : public SoundPlayer
    {
        virtual CoreSound* CreateSoundFromAsset(const String& filename) 
        {
            return new CoreSound(filename, true);
        }
        virtual SoundChannel* PlaySound(Sound* sound, bool loop)
        {
            CoreSoundChannel* result = new CoreSoundChannel(sound->GetPath().Ptr(), loop, true);
            return result;
        }        
    };

    SoundPlayer* SoundPlayer::Create()
    {
        return new CoreSoundPlayer();
    }
}
