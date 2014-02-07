#include <Xli/BufferStream.h>
#include <cstring>

namespace Xli
{
    BufferStream::BufferStream(DataAccessor* buf, bool canRead, bool canWrite)
    {
        if (!buf) 
            XLI_THROW_NULL_POINTER;
        
        this->buf = buf;
        this->buf->AddRef();
        this->pos = 0;
        this->read = canRead;
        this->write = canWrite;
        this->closed = false;
    }

    BufferStream::~BufferStream()
    {
        buf->Release();
    }

    void BufferStream::Close()
    {
        closed = true;
    }

    bool BufferStream::IsClosed() const
    {
        return closed;
    }

    bool BufferStream::AtEnd() const
    {
        if (closed) 
            XLI_THROW_STREAM_CLOSED;
        
        return pos == buf->GetSizeInBytes();
    }

    bool BufferStream::CanRead() const
    {
        return read;
    }

    bool BufferStream::CanWrite() const
    {
        return write;
    }

    bool BufferStream::CanSeek() const
    {
        return true;
    }

    int BufferStream::GetLength() const
    {
        if (closed) 
            XLI_THROW_STREAM_CLOSED;
        
        return buf->GetSizeInBytes();
    }

    int BufferStream::GetPosition() const
    {
        if (closed) 
            XLI_THROW_STREAM_CLOSED;
        
        return pos;
    }

    int BufferStream::Read(void* data, int elementSize, int elementCount)
    {
        if (!read) 
            XLI_THROW_STREAM_CANT_READ;
        
        if (closed) 
            XLI_THROW_STREAM_CLOSED;

        int bytes = elementCount * elementSize;
        int length = buf->GetSizeInBytes();

        if (bytes > length - pos)
        {
            bytes = length - pos;
            bytes -= bytes % elementSize;
            elementCount = bytes / elementSize;
        }
        else if (bytes < 0) 
        {
            bytes = 0;
            elementCount = 0;
        }
        
        memcpy(data, buf->GetDataPtr() + pos, bytes);
        pos += bytes;

        return elementCount;
    }

    int BufferStream::Write(const void* data, int elementSize, int elementCount)
    {
        if (!write) 
            XLI_THROW_STREAM_CANT_READ;

        if (closed) 
            XLI_THROW_STREAM_CLOSED;

        int bytes = elementCount * elementSize;
        int length = buf->GetSizeInBytes();

        if (bytes > length - pos)
        {
            bytes = length - pos;
            bytes -= bytes % elementSize;
            elementCount = bytes / elementSize;
        }
        else if (bytes < 0) 
        {
            bytes = 0;
            elementCount = 0;
        }
        
        memcpy(const_cast<UInt8*>(buf->GetDataPtr()) + pos, data, bytes);
        pos += bytes;

        return elementCount;
    }

    void BufferStream::Seek(SeekOrigin origin, int offset)
    {
        if (closed) 
            XLI_THROW_STREAM_CLOSED;

        int len = buf->GetSizeInBytes();

        switch (origin)
        {
        case SeekOriginBegin:
            pos = offset;
            break;

        case SeekOriginCurrent:
            pos += offset;
            break;

        case SeekOriginEnd:
            pos = len + offset;
            break;
        }

        if (pos < 0) 
            pos = 0;
        else if (pos > len)
            pos = len;
    }
}
