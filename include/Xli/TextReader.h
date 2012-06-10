#ifndef __XLI_TEXT_READER_H__
#define __XLI_TEXT_READER_H__

#include <Xli/Stream.h>

namespace Xli
{
	class TextReader: public StreamReader
	{
	public:
		TextReader(Stream* stream);

		CharString Read(int len);
		CharString ReadAll();
		char ReadChar();
		CharString ReadTo(char terminal);
		CharString ReadLine();
		bool AtEndOfFile();
	};
}

#endif
