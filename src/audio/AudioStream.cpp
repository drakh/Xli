#include <XliAudio/AudioStream.h>

namespace Xli
{
	class DefaultAudioStream: public AudioStream
	{
		Shared<Stream> source;
		DataType type;
		int channels;
		double sampleRate;

	public:
		DefaultAudioStream(Stream* source, DataType dataType, int channelCount, double sampleRate)
		{
			this->source = source;
			this->type = dataType;
			this->channels = channelCount;
			this->sampleRate = sampleRate;
		}

		virtual double GetSampleRate() const
		{
			return sampleRate;
		}
		
		virtual int GetChannelCount() const
		{
			return channels;
		}

		virtual DataType GetDataType() const
		{
			return type;
		}

		virtual int Read(void* dst, int elmSize, int elmCount)
		{
			return source->Read(dst, elmSize, elmCount);
		}
	};

	int AudioStream::GetAverageBytesPerSecond() const
	{
		return (int)((double)(DataTypeInfo::SizeOf(GetDataType()) * GetChannelCount()) * GetSampleRate());
	}

	AudioStream* AudioStream::Create(Stream* source, DataType dataType, int channelCount, double sampleRate)
	{
		return new DefaultAudioStream(source, dataType, channelCount, sampleRate);
	}
}
