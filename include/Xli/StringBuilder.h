#ifndef __XLI_STRING_BUILDER_H__
#define __XLI_STRING_BUILDER_H__

#include "String.h"
#include "Array.h"

namespace Xli
{
	template <typename TChar> class StringBuildert //: public Object
	{
		Array<TChar> buffer;

	public:
		StringBuildert(int cap = 1024) 
		{ 
			buffer.Reserve(cap);
		}
		StringBuildert& operator << (const Stringt<TChar>& text)
		{
			Append(text);
			return *this;
		}
		StringBuildert& operator << (const char* str)
		{
			Append(str);
			return *this;
		}
		StringBuildert& operator += (const Stringt<TChar>& text)
		{
			Append(text);
			return *this;
		}
		StringBuildert& operator += (const char* str)
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
			while (*str) buffer.Add((TChar)*str++);
		}
		void Append(const Stringt<TChar>& text)
		{
			buffer.Add(text.Data(), text.Length());
		}
		void AppendChar(TChar c)
		{
			buffer.Add(c);
		}
		void AppendLine(const char* str)
		{
			Append(str);
			buffer.Add('\n');
		}
		void AppendLine(const Stringt<TChar>& text)
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
		Stringt<TChar> GetString() const
		{
			return Stringt<TChar>(buffer.Data(), buffer.Length());
		}
		operator Stringt<TChar>() const
		{
			return GetString();
		}
		void AppendFormat(const char* format, ...)
		{
			va_list argList;
			va_start(argList, format);
			CharString s = StringTools::Format(format, argList);
			va_end(argList);
			Append(s);
		}
		/*virtual String ToString()
		{
			return GetString();
		}*/
	};

	typedef StringBuildert<Utf16Char> Utf16StringBuilder;
	typedef StringBuildert<char> CharStringBuilder;

//#ifdef XLI_UNICODE
	typedef Utf16StringBuilder StringBuilder;
//#else
//	typedef CharStringBuilder StringBuilder;
//#endif
}

#endif
