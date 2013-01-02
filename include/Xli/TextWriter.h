#ifndef __XLI_TEXT_WRITER_H__
#define __XLI_TEXT_WRITER_H__

#include <Xli/Stream.h>
#include <Xli/String.h>
#include <Xli/File.h>

namespace Xli
{
	/**
		\addtogroup Core
		@{
	*/

	CharString ToString(void* p);
	CharString ToString(int i);
	CharString ToString(unsigned int i);
	CharString ToString(short i);
	CharString ToString(unsigned short i);
	CharString ToString(long i);
	CharString ToString(unsigned long i);
	CharString ToString(long long i);
	CharString ToString(unsigned long long i);
	CharString ToString(float f);
	CharString ToString(double d);
	CharString ToString(char c);
	CharString ToString(unsigned char c);
	CharString ToString(bool b);

	template <class T> String ToString(const T& t)
	{
		return t.ToString();
	}

	template <typename T, uint TBufSize> const Stringt<T, TBufSize>& ToString(const Stringt<T, TBufSize>& str)
	{
		return str;
	}

	/** @} */


	/**
		\ingroup IO
	*/
	class TextWriter: public StreamWriter
	{
	public:
		TextWriter(Stream* stream);

		void Write(const char* str, int len);
		void Writef(const char* format, ...);
		void Write(const char* str);
		void Write(const CharString& str);
        void Write(const Utf16String& str);
		void WriteLine();

		template <typename T> void Write(const T& t)
		{
			Write(Xli::ToString(t));
		}
		template <typename T> void WriteLine(const T& t)
		{
			Write(t);
			WriteLine();
		}
		template <typename T> void Write(const Array<T>& a)
		{
			for (int i = 0; i < a.Length(); i++) WriteLine(a[i]);
		}
	};
}

#endif
