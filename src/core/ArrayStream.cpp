#include <Xli/ArrayStream.h>
#include <cstring>

namespace Xli
{
    ArrayStream::ArrayStream(int initCapacity)
    {
        buf.Reserve(initCapacity);
    }

    bool ArrayStream::CanWrite() const
    {
        return true;
    }

    int ArrayStream::Write(const void* src, int elmSize, int elmCount)
    {
        int loc = buf.Length();
        buf.Expand();
        buf.Resize(buf.Length() + elmSize * elmCount);
        memcpy(buf.DataPtr() + loc, src, elmSize * elmCount);
        return elmCount;
    }

    int ArrayStream::GetLength() const
    {
        return buf.Length();
    }

    const UInt8* ArrayStream::GetDataPtr() const
    {
        return buf.DataPtr();
    }

    Buffer* ArrayStream::CopyData() const
    {
        return Buffer::CopyFrom(buf.DataPtr(), buf.Length());
    }

    void ArrayStream::Clear()
    {
        buf.Clear();
    }
}
