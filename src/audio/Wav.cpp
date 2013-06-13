#include <XliAudio/Wav.h>
#include <Xli/BinaryReader.h>

namespace Xli
{
	class WavAudioStream: public AudioStream
	{
		Shared<Stream> source;
		DataType type;
		int channels;
		double sampleRate;
		int dataChunkPos, dataChunkLength;

	public:
		WavAudioStream(Stream* source)
		{
			this->source = source;
			if (!source->CanRead()) XLI_THROW("Source must be readable");

			BinaryReader r(source);

			if (r.ReadCStr(4) != "RIFF") XLI_THROW("Invalid WAV-file: RIFF-header not found");

			int totalLength = r.ReadInt32();

			if (r.ReadCStr(4) != "WAVE") XLI_THROW("Invalid WAV-file: WAVE-header not found");
			if (r.ReadCStr(3) != "fmt") XLI_THROW("Invalid WAV-file: FORMAT-header not found");

			r.ReadInt8();
			r.ReadInt32();
			r.ReadInt16();

			this->channels = r.ReadInt16();
			this->sampleRate = (double)r.ReadInt32();
			r.ReadInt32(); // bytesPerSecond
			r.ReadInt16(); // bytesPerSample
			int bitsPerSample = r.ReadInt16();

			switch (bitsPerSample)
			{
			case 8: this->type = DataTypeUInt8; break;
			case 16: this->type = DataTypeInt16; break;
			case 24: this->type = DataTypeInt24; break;
			case 32: this->type = DataTypeInt32; break;
			default: XLI_THROW("Unsupported bits per sample: " + (String)(int)bitsPerSample);
			}

			// wait for DATA chunk
			while (source->GetPosition() < totalLength)
			{
				String tag = r.ReadCStr(4);
				int length = r.ReadInt32();
				if (tag == "data")
				{
					this->dataChunkPos = source->GetPosition();
					this->dataChunkLength = length;
					return;
				}
				source->Seek(SeekOriginCurrent, length);
			}

			XLI_THROW("Invalid WAV-file: DATA-header not found");
		}

		virtual bool CanRead() const { return true; }
		virtual bool CanSeek() const { return source->CanSeek(); }

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

		virtual void Seek(SeekOrigin origin, int offset)
		{
			int currentPos = GetPosition();
			int blockAlignedOffset = offset - (offset % (DataTypeInfo::SizeOf(type) * channels));
			int newPos;
			switch (origin)
			{
			case SeekOriginBegin:
			default:
				newPos = offset;
				break;
			case SeekOriginCurrent: newPos = currentPos + offset; break;
			case SeekOriginEnd: newPos = dataChunkLength + offset; break;
			}
			if (newPos < 0) newPos = 0;
			if (newPos > dataChunkLength) newPos = dataChunkLength;
			source->Seek(SeekOriginBegin, dataChunkPos + newPos);
		}
		virtual int GetPosition() const { return source->GetPosition() - dataChunkPos; }
		virtual int GetLength() const { return dataChunkLength; }
	};

	AudioStream* Wav::Open(Stream* f)
	{
		return new WavAudioStream(f);
	}
}
