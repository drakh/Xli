#include <XliSound/WavStream.h>
#include <Xli/BinaryReader.h>

namespace Xli
{
	WavStream::WavStream(Stream* f)
	{
		BinaryReader r(f);

		if (r.ReadCStr(4) != "RIFF") XLI_THROW("Invalid WAV-file: RIFF-header not found");

		int totalLength = r.ReadInt32();

		if (r.ReadCStr(4) != "WAVE") XLI_THROW("Invalid WAV-file: WAVE-header not found");
		if (r.ReadCStr(3) != "fmt") XLI_THROW("Invalid WAV-file: FORMAT-header not found");

		r.ReadInt8();
		r.ReadInt32();
		r.ReadInt16();

		int channels = r.ReadInt16();
		int sampleRate = r.ReadInt32();
		int bytesPerSecond = r.ReadInt32();
		int bytesPerSample = r.ReadInt16();
		int bitsPerSample = r.ReadInt16();

		this->f = f;
		this->channels = channels;
		this->sampleRate = (double)sampleRate;
		
		switch (bitsPerSample)
		{
		case 8: this->dataType = DataTypeUInt8; break;
		case 16: this->dataType = DataTypeInt16; break;
		case 24: this->dataType = DataTypeInt24; break;
		case 32: this->dataType = DataTypeInt32; break;
		default: XLI_THROW("Unsupported bits per sample: " + (String)(int)bitsPerSample);
		}


		// wait for DATA chunk
		while (f->GetPosition() < totalLength)
		{
			CharString tag = r.ReadCStr(4);
			int length = r.ReadInt32();
			if (tag == "data") return;
			f->Seek(SeekOriginCurrent, length);
		}

		XLI_THROW("Invalid WAV-file: DATA-header not found");
	}
}
