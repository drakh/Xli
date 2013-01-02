#ifndef __XLI_TEXT_H__
#define __XLI_TEXT_H__

#include "TextReader.h"
#include "TextWriter.h"

namespace Xli
{
	class Text
	{
	public:
		static CharString LoadRaw(const String& fileName);
		static Utf16String Load(const String& fileName);
		static void Save(const String& fileName, const CharString& content);
		static void Save(const String& fileName, const Utf16String& content);
	};
}

#endif
