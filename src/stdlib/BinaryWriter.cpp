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

	void BinaryWriter::WriteString(const Utf16String& str)
	{
		CharString utf8 = Unicode::Utf16To8(str);
		Write7BitEncodedInt(utf8.Length());
		stream->WriteSafe(utf8.Data(), 1, utf8.Length());
	}
}
