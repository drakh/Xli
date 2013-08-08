#ifndef __XLI_TEXT_READER_H__
#define __XLI_TEXT_READER_H__

#include <Xli/Stream.h>

namespace Xli
{
	/**
		\ingroup XliCoreIO
	*/
	class TextReader: public StreamReader
	{
	public:
		TextReader(Stream* stream);

		String Read(int len);
		String ReadAll();
		char ReadChar();
		String ReadTo(char terminal);
		String ReadLine();
		bool AtEndOfFile();
	};
}

#endif
