#ifndef __XLI_STRING_BUILDER_H__
#define __XLI_STRING_BUILDER_H__

#include "String.h"
#include "Array.h"
#include "Unicode.h"

namespace Xli
{
	/**
		\ingroup Core
	*/
	class StringBuilder
	{
		Array<StringChar> buffer;

	public:
		StringBuilder(int cap = 1024) 
		{ 
			buffer.Reserve(cap);
		}
		StringBuilder& operator << (const String& text)
		{
			Append(text);
			return *this;
		}
		StringBuilder& operator << (const char* str)
		{
			Append(str);
			return *this;
		}
		StringBuilder& operator += (const String& text)
		{
			Append(text);
			return *this;
		}
		StringBuilder& operator += (const char* str)
		{
			Append(str);
			return *this;
		}
		int GetLength() const
		{ 
			return buffer.Length(); 
		}
		void Append(const char* str)
		{
			Append(Unicode::Utf8To16(CharString(str)));
		}
		void Append(const String& text)
		{
			buffer.Add(text.Data(), text.Length());
		}
		void AppendChar(StringChar c)
		{
			buffer.Add(c);
		}
		void AppendLine(const char* str)
		{
			Append(str);
			buffer.Add('\n');
		}
		void AppendLine(const String& text)
		{
			Append(text);
			buffer.Add('\n');
		}
		void AppendLine()
		{
			buffer.Add('\n');
		}
		void Clear()
		{
			buffer.Clear();
		}
		String GetString() const
		{
			return String(buffer.Data(), buffer.Length());
		}
		operator String() const
		{
			return GetString();
		}
		void AppendFormat(const char* format, ...)
		{
			va_list argList;
			va_start(argList, format);
			CharString s = StringTools::Format(format, argList);
			va_end(argList);
			Append(Unicode::Utf8To16(s));
		}
	};
}

#endif
