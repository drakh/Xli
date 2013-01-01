#ifndef __XLI_SOUND_WAV_STREAM_H__
#define __XLI_SOUND_WAV_STREAM_H__

#include <Xli/Stream.h>
#include <Xli/DataType.h>
#include <Xli/File.h>

namespace Xli
{
	class WavStream: public Stream
	{
		Shared<Stream> f;

		double sampleRate;
		int channels;
		DataType dataType;

	public:
		WavStream(Stream* f);

		virtual double GetSampleRate() const { return sampleRate; }
		virtual int GetChannelCount() const { return channels; }
		virtual DataType GetDataType() const { return dataType; }

		virtual int Read(void* dst, int elmSize, int elmCount) { return f->Read(dst, elmSize, elmCount); }
	};

	class Wav
	{
	public:
		static WavStream* Open(const String& fileName)
		{
			Managed<File> f = new File(fileName, FileModeRead);
			return new WavStream(f);
		}
	};
}

#endif
