#include <Xli/TextWriter.h>
#include <Xli/Unicode.h>

namespace Xli
{
	CharString ToString(void* p)
	{
		char buf[64];
		sprintf_s(buf, 64, "0x%llx", (unsigned long long int)p);
		return buf;
	}
	CharString ToString(int i)
	{
		return i;
	}
	CharString ToString(unsigned int i)
	{
		char buf[64];
		sprintf_s(buf, 64, "%u", i);
		return buf;
	}
	CharString ToString(short i)
	{
		char buf[64];
		sprintf_s(buf, 64, "%hd", i);
		return buf;
	}
	CharString ToString(unsigned short i)
	{
		char buf[64];
		sprintf_s(buf, 64, "%hu", i);
		return buf;
	}
	CharString ToString(long i)
	{
		char buf[64];
		sprintf_s(buf, 64, "%ld", i);
		return buf;
	}
	CharString ToString(unsigned long i)
	{
		char buf[64];
		sprintf_s(buf, 64, "%lu", i);
		return buf;
	}
	CharString ToString(long long i)
	{
		char buf[64];
		sprintf_s(buf, 64, "%lld", i);
		return buf;
	}
	CharString ToString(unsigned long long i)
	{
		char buf[64];
		sprintf_s(buf, 64, "%llu", i);
		return buf;
	}
	CharString ToString(float f)
	{
		return f;
	}
	CharString ToString(double d)
	{
		return d;
	}
	CharString ToString(char c)
	{
		return CharString(&c, 1);
	}
	CharString ToString(unsigned char c)
	{
		return CharString(&c, 1);
	}
	CharString ToString(bool b)
	{
		return b ? "true" : "false";
	}

	TextWriter::TextWriter(Stream* stream): StreamWriter(stream)
	{
	}

	void TextWriter::Write(const char* str, int len)
	{
		stream->WriteSafe(str, 1, len);
	}

	void TextWriter::Writef(const char* format, ...)
	{
		va_list argList;
		va_start(argList, format);
		WriteString(StringTools::Format(format, argList));
		va_end(argList);
	}

	void TextWriter::Write(const char* str)
	{
		Write(str, strlen(str));
	}

	void TextWriter::WriteString(const CharString& str)
	{
		Write(str.Data(), str.Length());
	}
    
	void TextWriter::WriteString(const Utf16String& str)
	{
		WriteString(Unicode::Utf16To8(str));
	}

	void TextWriter::WriteLine()
	{
		stream->WriteSafe("\n", 1, 1);
	}
}
