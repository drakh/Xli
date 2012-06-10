#include <Xli/BinaryWriter.h>
#include <Xli/Unicode.h>

namespace Xli
{
	BinaryWriter::BinaryWriter(Stream* stream): StreamWriter(stream)
	{
	}

	BinaryWriter::~BinaryWriter()
	{
	}

	void BinaryWriter::Write(const void* data, int elmSize, int elmCount)
	{
		stream->WriteSafe(data, elmSize, elmCount);
	}

	void BinaryWriter::WriteCStr(const char* str)
	{
		stream->WriteSafe((const void*)str, 1, strlen(str));
	}

	void BinaryWriter::Write7BitEncodedInt(int value) 
	{
        unsigned int v = (unsigned int) value;
        
		while (v >= 0x80)
		{
            WriteUInt8((v | 0x80));
			v >>= 7;
        }

        WriteUInt8(v);
    }

	void BinaryWriter::WriteUtf8StringData(const Utf16String& str)
	{
		WriteCharStringData(Unicode::Utf16To8(str));
	}

	void BinaryWriter::WriteEncodedUtf16String(const Utf16String& str)
	{
		Write7BitEncodedInt(str.Length());
		WriteUtf8StringData(str);
	}
}
