#ifndef __XLI_TEXT_H__
#define __XLI_TEXT_H__

#include "TextReader.h"
#include "TextWriter.h"

namespace Xli
{
	class Text
	{
	public:
		static String Load(const String& fileName);
		static void Save(const String& fileName, const String& content);
	};
}

#endif
