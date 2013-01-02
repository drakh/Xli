#ifndef __XLI_TEXT_READER_H__
#define __XLI_TEXT_READER_H__

#include <Xli/Stream.h>

namespace Xli
{
	/**
		\ingroup IO
	*/
	class TextReader: public StreamReader
	{
	public:
		TextReader(Stream* stream);

		CharString ReadRaw(int len);
		CharString ReadAllRaw();
		Utf16String ReadAll();
		char ReadChar();
		CharString ReadToRaw(char terminal);
		CharString ReadLineRaw();
		Utf16String ReadLine();
		bool AtEndOfFile();
	};
}

#endif
