#ifndef __XLI_TEXT_LEXER_H__
#define __XLI_TEXT_LEXER_H__

#include <Xli/String.h>

namespace Xli
{
	class TextParser
	{
		const char* data;
		int len, start;

	public:
		TextParser(const char* data, int len);

		bool AtEnd() const;
		bool Match(const char* head) const;

		void SkipSpace();
		void SkipToken();
		void SkipLine();

		int ParseInt();

		CharString NextToken();
		CharString NextLine();

		int ParseDataToInt(const char* lineStart);
		CharString ParseDataToLine(const char* lineStart);
		CharString ParseDataToToken(const char* lineStart);
	};
}

#endif
