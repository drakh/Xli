#include <Xli/BinaryReader.h>

namespace Xli
{
    BinaryReader::BinaryReader(Stream* stream)
        : StreamReaderBase(stream)
    {
    }

    BinaryReader::~BinaryReader()
    {
    }

    void BinaryReader::Read(void* dst, int elmSize, int elmCount)
    {
        _stream->ReadSafe(dst, elmSize, elmCount);
    }

    String BinaryReader::ReadCStr(int len)
    {
        String str = String::Create(len);
        _stream->ReadSafe((void*)str.DataPtr(), 1, len);
        return str;
    }

    String BinaryReader::ReadString()
    {
        return ReadCStr(Read7BitEncodedInt32());
    }

    Buffer* BinaryReader::ReadAll()
    {
        _stream->Seek(0, SeekOriginBegin);
        int size = _stream->GetLength();
        Buffer* buf = Buffer::Create(size);
        _stream->ReadSafe(buf->DataPtr(), 1, size);
        return buf;
    }

    Int32 BinaryReader::Read7BitEncodedInt32()
    {
        // TODO: Protect against forever while loop (ref: .NET impl)

        int count = 0;
        int shift = 0;
        int b;
        
        do 
        {
            b = ReadUInt8();
            count |= (b & 0x7F) << shift;
            shift += 7;
        }
        while ((b & 0x80) != 0);
        
        return count;
    }
}
