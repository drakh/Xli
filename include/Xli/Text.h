#ifndef __XLI_TEXT_H__
#define __XLI_TEXT_H__

#include "TextReader.h"
#include "TextWriter.h"

namespace Xli
{
	class Text
	{
	public:
		static String Load(const String& filename);
		static void Save(const String& filename, const String& content);
	};
}

#endif
