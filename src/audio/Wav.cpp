#include <XliAudio/Wav.h>
#include <Xli/BinaryReader.h>

namespace Xli
{
	AudioStream* Wav::Open(Stream* f)
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
		r.ReadInt32(); // bytesPerSecond
		r.ReadInt16(); // bytesPerSample
		int bitsPerSample = r.ReadInt16();
		
		DataType dataType;
		switch (bitsPerSample)
		{
		case 8: dataType = DataTypeUInt8; break;
		case 16: dataType = DataTypeInt16; break;
		case 24: dataType = DataTypeInt24; break;
		case 32: dataType = DataTypeInt32; break;
		default: XLI_THROW("Unsupported bits per sample: " + (String)(int)bitsPerSample);
		}

		// wait for DATA chunk
		while (f->GetPosition() < totalLength)
		{
			String tag = r.ReadCStr(4);
			int length = r.ReadInt32();
			if (tag == "data") return AudioStream::Create(f, dataType, channels, sampleRate);
			f->Seek(SeekOriginCurrent, length);
		}

		XLI_THROW("Invalid WAV-file: DATA-header not found");
	}
}
