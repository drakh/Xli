#include <Xli/TextReader.h>
#include <Xli/File.h>
#include <Xli/Unicode.h>

namespace Xli
{
	TextReader::TextReader(Stream* stream): StreamReader(stream)
	{
	}

	CharString TextReader::ReadRaw(int len)
	{
		CharString s = CharString::Create(len);
		stream->ReadSafe(s.Data(), 1, len);
		return s;
	}

	CharString TextReader::ReadAllRaw()
	{
		if (stream->CanSeek())
		{
			return ReadRaw(stream->GetLength());
		}
		
		Array<char> str;

		char buf[1024];
		int len = 0;
		
		while ((len = stream->Read(buf, 1, 1024)) > 0)
		{
			str.Add(buf, len);
		}
		
		return CharString(str.Data(), str.Length());
	}

	Utf16String TextReader::ReadAll()
	{
		return Unicode::Utf8To16(ReadAllRaw());
	}

	char TextReader::ReadChar()
	{
		char c;
		stream->ReadSafe(&c, 1, 1);
		return c;
	}

	CharString TextReader::ReadToRaw(char terminal)
	{
		Array<char> s;
		char c;
		
		while (stream->Read(&c, 1, 1) == 1)
		{
			s.Add(c);
			if (c == terminal) break;
		}

		return CharString(s.Data(), s.Length());
	}

	CharString TextReader::ReadLineRaw()
	{
		return ReadToRaw('\n');
	}

	Utf16String TextReader::ReadLine()
	{
		return Unicode::Utf8To16(ReadLineRaw());
	}

	bool TextReader::AtEndOfFile()
	{
		return stream->AtEnd();
	}
}
