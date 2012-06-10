#include <Xli/Unicode.h>
#include <Xli/BinaryReader.h>

namespace Xli
{
	BinaryReader::BinaryReader(Stream* stream): StreamReader(stream)
	{
	}

	BinaryReader::~BinaryReader()
	{
	}

	void BinaryReader::Read(void* dst, int elmSize, int elmCount)
	{
		stream->ReadSafe(dst, elmSize, elmCount);
	}

	CharString BinaryReader::ReadCStr(int len)
	{
		CharString str = CharString::Create(len);
		stream->ReadSafe((void*)str.Data(), 1, len);
		return str;
	}

	Utf16String BinaryReader::ReadUtf8String(int len)
	{
		return Xli::Unicode::Utf8To16(ReadCStr(len));
	}

	Buffer* BinaryReader::ReadBuffer()
	{
		UInt32 size = ReadUInt32();
		Buffer* buf = Buffer::Create(size);
		stream->ReadSafe(buf->Data(), 1, size);
		return buf;
	}

	Buffer* BinaryReader::ReadAll()
	{
		stream->Seek(SeekOriginBegin, 0);
		UInt32 size = stream->GetLength();
		Buffer* buf = Buffer::Create(size);
		stream->ReadSafe(buf->Data(), 1, size);
		return buf;
	}

    int BinaryReader::Read7BitEncodedInt()
	{
		int count = 0;
		int shift = 0;
		int b;
		
		do 
		{
			b = ReadUInt8();
			count |= (b & 0x7F) << shift;
			shift += 7;
		} while ((b & 0x80) != 0);
		
		return count;
	}


}
