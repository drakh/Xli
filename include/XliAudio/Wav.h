#ifndef __XLI_AUDIO_WAV_H__
#define __XLI_AUDIO_WAV_H__

#include <XliAudio/AudioStream.h>
#include <Xli/File.h>

namespace Xli
{
	/**
		\ingroup Audio
	*/
	class Wav
	{
	public:
		static AudioStream* Open(Stream* source);
		
		static AudioStream* Open(const String& fileName)
		{
			Managed<File> f = new File(fileName, FileModeRead);	
			return Open(f);
		}
	};
}

#endif
