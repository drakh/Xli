#include "Sound.h"

#include <assert.h>
#include <jni.h>
#include <string.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
extern struct AAssetManager* XliAAssetManager;

#include <XliPlatform.h>
#include <Xli/Exception.h>

namespace Uno
{
	namespace Audio
	{
		static Channel** channels = 0;
		static int channelCount = 0;
		static const int maxChannels = 32;

		static int findFreeChannel()
		{
			// decrement channelCount so it points to 1 + the index of the last used channel pointer 
			while (channelCount > 1 && channels[channelCount - 1] == 0)
			{
				channelCount--;
			}

			// mark pointer places for channels we no longer need as free
			for (int i = 0; i < channelCount; i++)
			{
				if (channels[i] != 0)
				{
					if (channels[i]->IsFinished())
					{
						channels[i]->Stop();
						channels[i] = 0;
					}
				}
			}

			// find a free pointer place for the pointer to our new channel and return it if found
			for (int i=0; i<channelCount; i++)
			{
				if (channels[i] == 0)
				{
					return i;
				}
				/*else
				{
					Xli::PrintLine(Xli::CharString::Format("%d %d %d %d %.2f %.2lf %.2lf", i, channels[i]->GetPaused() ? 1 : 0, channels[i]->IsPlaying() ? 1 : 0, channels[i]->IsFinished() ? 1 : 0, channels[i]->GetVolume(), channels[i]->GetPosition(), channels[i]->GetDuration()));
				}*/
			}

			// if we didn't find any channel among the used ones, and we're really out of channels, return -1
			if (channelCount == maxChannels)
			{
				return -1;
			}

			// otherwise we'll just return the next channel and increment channelCount
			return channelCount++;
		}

		static Channel* dummyChannel;

		class Channel: public Object
		{
			SLObjectItf fdPlayerObject;
			SLPlayItf fdPlayerPlay;
			SLSeekItf fdPlayerSeek;
			SLVolumeItf fdPlayerVolume;

			bool stopping;

			static void PlayCallback(SLPlayItf caller, void* context, SLuint32 event);

		public:
			Channel() { }
			Channel(int fd, off_t start, off_t length);
			virtual ~Channel();

			virtual double GetPosition();
			virtual void SetPosition(double value);

			virtual float GetPan();
			virtual void SetPan(float value);

			virtual float GetVolume();
			virtual void SetVolume(float value);

			virtual bool GetPaused();
			virtual void SetPaused(bool value);

			virtual bool IsPlaying();
			virtual bool IsFinished();

			virtual void Stop();

			virtual double GetDuration();

			virtual void Play();
			virtual void PlayLoop();
		};

		class DummyChannel: public Channel
		{
		public:
			DummyChannel() { }
			virtual ~DummyChannel() {}

			virtual double GetPosition() { return 0.0; }
			virtual void SetPosition(double value) { }

			virtual float GetPan() { return 0.0f; }
			virtual void SetPan(float value) { }

			virtual float GetVolume() { return 0.0f; }
			virtual void SetVolume(float value) { }

			virtual bool GetPaused() { return false; }
			virtual void SetPaused(bool value) { }

			virtual bool IsPlaying() { return false; }
			virtual bool IsFinished() { return true; }

			virtual void Stop() { }

			virtual double GetDuration() { return 0.0; }

			virtual void Play() { }
			virtual void PlayLoop() { }
		};

		class SlesSound: public Object
		{
			off_t start, length;
			int fd;

			double duration;

		public:
			Sound(const char* filename, PlayMode mode);
			virtual ~Sound();

			double GetDuration();

			Channel* Play();
			Channel* PlayLoop();
		};

		Sound::Sound(const char* fn, PlayMode mode)
		{
			Xli::CharString filename = fn;
			Xli::PrintLine("Loading: " + filename);

			if (channels == 0)
			{
				channels = new Channel*[maxChannels];
				memset(channels, 0, maxChannels * sizeof(Channel*));
				uno_gc_register_static_region(&channels, maxChannels * sizeof(Channel*));
				dummyChannel = new DummyChannel();
				uno_gc_register_static_region(&dummyChannel, sizeof(Channel*)); // XXX: we need to set it to 0 somewhere (atexit)
			}

			AAsset* asset = AAssetManager_open(XliAAssetManager, (const char*)filename.Data(), AASSET_MODE_UNKNOWN);

			if (!asset)
			{
				XLI_THROW("Unable to open asset: " + filename);
			}

			// open asset as file descriptor
			start, length;
			fd = AAsset_openFileDescriptor(asset, &start, &length);

			if (fd < 0)
			{
				XLI_THROW("Unable to open file descriptor: " + filename);
			}
			
			AAsset_close(asset);

			Channel* channel = new Channel(fd, start, length);
			this->duration = channel->GetDuration();
			delete channel;
		}

		Sound::~Sound()
		{
		}

		double Sound::GetDuration()
		{
			return duration;
		}

		Channel* Sound::Play()
		{
			int channelIndex = findFreeChannel();
			if (channelIndex == -1)
			{
				Xli::ErrorPrintLine("OPENSL ERROR: Out of channels, you won't hear any sounds for a while");
				return dummyChannel;
			}

			Channel* result = new (uno_gc) Channel(fd, start, length);
			result->Play();
			channels[channelIndex] = result;

			return result;
		}

		Channel* Sound::PlayLoop()
		{
			int channelIndex = findFreeChannel();
			if (channelIndex == -1)
			{
				Xli::ErrorPrintLine("OPENSL ERROR: Out of channels, you won't hear any sounds for a while");
				return dummyChannel;
			}
			
			Channel* result = new (uno_gc) Channel(fd, start, length);
			result->PlayLoop();
			channels[channelIndex] = result;

			return result;
		}
	}
}
