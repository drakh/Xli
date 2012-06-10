#ifndef __XLI_TEXT_H__
#define __XLI_TEXT_H__

#include "TextReader.h"
#include "TextWriter.h"
#include "List.h"

namespace Xli
{
	class Text
	{
	public:
		static CharString Load(const CharString& fileName);
		static List<CharString> LoadLines(const CharString& fileName);
		static void Save(const CharString& fileName, const CharString& content);
	};
}

#endif
