#include <XliAudio/SampleReader.h>

namespace Xli
{
	float SampleReader::convertSample(Int16 sample)
	{
		return (float)((double)sample / 32768.0);
	}

	SampleReader::SampleReader(AudioStream* stream, bool forceStereo)
	{
		this->stream = stream;

		if (stream->GetChannelCount() > 2) XLI_THROW("Sample reader stream can only have one or two channels");
		if (stream->GetDataType() != Xli::DataTypeInt16) XLI_THROW("Sample reader can only read Int16 streams");

		this->forceStereo = forceStereo;
		this->internalBufferSize = 512;
		this->internalBuffer = new Int16[this->internalBufferSize];
	}

	SampleReader::~SampleReader()
	{
		delete [] internalBuffer;
	}

	int SampleReader::Read(float* buffer, int expectedSamples)
	{
		int samplesRead = 0;
		int bufferOffset = 0;
		while (samplesRead < expectedSamples)
		{
			int samplesToRead = Min(expectedSamples - samplesRead, internalBufferSize);
			int currentSamplesRead;
			if (stream->GetChannelCount() != 2 && forceStereo)
			{
				currentSamplesRead = stream->Read(internalBuffer, DataTypeInfo::SizeOf(DataTypeInt16), samplesToRead / 2);
				for (int i = 0; i < currentSamplesRead; i++)
				{
					float sample = convertSample(internalBuffer[i]);
					buffer[bufferOffset++] = sample;
					buffer[bufferOffset++] = sample;
				}
			}
			else
			{
				currentSamplesRead = stream->Read(internalBuffer, DataTypeInfo::SizeOf(DataTypeInt16), samplesToRead);
				for (int i = 0; i < currentSamplesRead; i++) buffer[bufferOffset++] = convertSample(internalBuffer[i]);
			}

			samplesRead += currentSamplesRead;
			if (currentSamplesRead < samplesToRead) break;
		}
		return samplesRead;
	}
}
