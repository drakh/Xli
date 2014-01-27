#include "Sound.h"
#include <XliAudio/SimpleAudio.h>
#include <Xli/PlatformSpecific/iOS.h>
//#include <assert.h>
#include <AudioToolbox/AudioToolbox.h>

#import <AVFoundation/AVFoundation.h>

namespace Xli
{
    class CASimpleSoundChannel : public SimpleSoundChannel
    {
        AVAudioPlayer* player;
            
    public:
        CASimpleSoundChannel(const char* filename, PlayMode mode, bool loop, bool play)
        {
            NSString* string = [NSString stringWithFormat:@"%@/data/%@", [[NSBundle mainBundle] resourcePath], [NSString stringWithUTF8String:filename]];
                
            NSURL* url = [NSURL fileURLWithPath:string];
            //[string release];
                
            NSError* error;
            player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
            //[url release];
                
            if (player == nil)
            {
                Xli::ErrorPrintLine("ERROR: Failed to create AVAudioPlayer: " + (Xli::CharString)([[error description] UTF8String]));
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
            
        virtual ~CASimpleSoundChannel()
        {
            if (player == nil) return;
            [player release];
        }
            
        double GetDuration()
        {
            if (player == nil) return 0.0;
            return player.duration;
        }
            
        virtual double GetPosition()
        {   
            if (player == nil) return 0.0;
            return player.currentTime;
        }
        virtual void SetPosition(double value)
        {
            if (player == nil) return;
            player.currentTime = value;
        }
            
        virtual float GetPan()
        {
            if (player == nil) return 0.0f;
            return player.pan;
        }
        virtual void SetPan(float value)
        {
            if (player == nil) return;
            player.pan = value;
        }
            
        virtual float GetVolume()
        {
            if (player == nil) return 0.0f;
            return player.volume;
        }
        virtual void SetVolume(float value)
        {
            if (player == nil) return;
            player.volume = value;
        }
            
        virtual bool GetPause()
        {
            if (player == nil) return true;
            return !player.playing;
        }
        virtual void SetPause(bool value)
        {
            if (player == nil) return;
            if (value) [player play];
            else [player pause];
        }
            
        virtual bool GetIsPlaying()
        {
            if (player == nil) return false;
            return player.playing;
        }
        virtual bool GetIsFinished()
        {
            if (player == nil) return true;
            return false;
        }
            
        virtual void Stop()
        {
            if (player == nil) return;
            [player stop];
            [player release];
            player = nil;
        }  
    };
}

