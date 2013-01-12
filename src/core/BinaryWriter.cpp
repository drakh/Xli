#include <Xli/BinaryWriter.h>
#include <cstring>

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
		stream->WriteSafe((const void*)str, 1, (int)strlen(str));
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

	void BinaryWriter::WriteString(const String& str)
	{
		Write7BitEncodedInt(str.Length());
		stream->WriteSafe(str.Data(), 1, str.Length());
	}
}
