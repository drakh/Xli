#ifndef __XLI_MEDIA_JSON_H__
#define __XLI_MEDIA_JSON_H__

#include <Xli/Value.h>
#include <Xli/File.h>
#include <Xli/TextReader.h>

namespace Xli
{
	class Json
	{
	public:
		/**
			Formats a string to be outputted to a JSON document.
			Converts to Utf8, adds quotes and escapes special characters.
		*/
		static CharString FormatString(const String& str);

		static Value Parse(const CharString& code, bool preserveOrder = false);

		inline static Value Load(Stream* stream, bool preserveOrder = false)
		{
			return Parse(TextReader(stream).ReadAll(), preserveOrder);
		}

		inline static Value Load(const String& fileName, bool preserveOrder = false)
		{
			File f(fileName, FileModeRead);
			return Load(&f, preserveOrder);
		}

		static void Save(Stream* stream, const Value& value);

		inline static void Save(const String& fileName, const Value& value)
		{
			File f(fileName, FileModeWrite);
			Save(&f, value);
		}

		static CharString ToString(const Value& value);
	};
}

#endif
