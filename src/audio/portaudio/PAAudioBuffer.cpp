#include <XliAudio/AudioBuffer.h>
#include <Xli/MessageBox.h>
#include <portaudio.h>

namespace Xli
{
	class PAAudioBuffer: public AudioBuffer
	{
		Shared<Stream> src;
		DataType dataType;
		int channels;
		double sampleRate;
		PaStream* stream;

		static int paCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *outTime, PaStreamCallbackFlags statusFlags, void *userData)
		{
			PAAudioBuffer* s = (PAAudioBuffer*)userData;
			
			int elmSize = DataTypeInfo::SizeOf(s->dataType);
			int elmCount = s->channels * framesPerBuffer;

			try
			{
				for (;;)
				{
					int result = s->src->Read(outputBuffer, elmSize, elmCount);

					if (result < elmCount)
					{
						elmCount -= result;
						outputBuffer = (UInt8*)outputBuffer + result * elmSize;

						if (s->src->AtEnd() || result == 0)
						{
							memset((UInt8*)outputBuffer, 0, elmCount * elmSize);
							return paComplete;
						}

						continue;
					}

					break;
				}

				return paContinue;
			}
			catch (const Exception& e)
			{
				memset((UInt8*)outputBuffer, 0, elmCount * elmSize);
				MessageBox::HandleException(e, "Sound: " + s->src->ToString());
				return paAbort;
			}
		}

		static PaSampleFormat XliDataTypeToPaSampleFormat(DataType dataType)
		{
			switch (dataType)
			{
			case DataTypeInt8: return paInt8;
			case DataTypeInt16: return paInt16;
			case DataTypeInt24: return paInt24;
			case DataTypeInt32: return paInt32;
			case DataTypeFloat: return paFloat32;
			case DataTypeUInt8: return paUInt8;
			default: XLI_THROW("Unsupported data type: " + (String)DataTypeInfo::ToString(dataType));
			}
		}

	public:
		PAAudioBuffer(Stream* source, DataType dataType, int channels, double sampleRate)
		{
			this->src = source;
			this->dataType = dataType;
			this->channels = channels;
			this->sampleRate = sampleRate;

			int err = Pa_OpenDefaultStream(&stream, 0, channels, XliDataTypeToPaSampleFormat(dataType), sampleRate, 0, paCallback, this);

			if (err)
			{
				XLI_THROW("Unable to open sound stream: " + (String)Pa_GetErrorText(err));
			}
		}

		virtual ~PAAudioBuffer()
		{
			Pa_CloseStream(stream);
		}

		virtual void Play()
		{
			int err = Pa_StartStream(stream);

			if (err)
			{
				XLI_THROW("Unable to start sound stream: " + (String)Pa_GetErrorText(err));
			}
		}
		
		virtual void Stop()
		{
			int err = Pa_StopStream(stream);

			if (err)
			{
				String str = "Unable to start sound stream: " + (String)Pa_GetErrorText(err);
				XLI_THROW(str);
			}
		}

		virtual double GetLatency() const
		{
			return Pa_GetStreamInfo(stream)->outputLatency;
		}

		virtual DataType GetDataType() const
		{
			return dataType;
		}

		virtual int GetChannelCount() const
		{
			return channels;
		}

		virtual double GetSampleRate() const
		{
			return sampleRate;
		}
	};

	static int SoundRefCount = 0;

	void AudioBuffer::Init()
	{
		if (SoundRefCount == 0)
		{
			int err = Pa_Initialize();

			if (err)
			{
				XLI_THROW("Unable to initialize PortAudio: " + (String)Pa_GetErrorText(err));
			}
		}

		SoundRefCount++;
	}

	void AudioBuffer::Shutdown()
	{
		SoundRefCount--;

		if (SoundRefCount == 0)
		{
			Pa_Terminate();
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
			atexit(AudioBuffer::Shutdown);
		}
	}

	AudioBuffer* AudioBuffer::Create(Stream* source, DataType dataType, int channels, double sampleRate)
	{
		AssertInit();
		return new PAAudioBuffer(source, dataType, channels, sampleRate);
	}
}
