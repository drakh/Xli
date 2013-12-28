#include <Xli/Stream.h>
#include <Xli/Console.h>
#include <Xli/File.h>

namespace Xli
{
    Stream::~Stream() 
    {
    }

    void Stream::Flush() 
    {
    }
    
    void Stream::Close() 
    {
    }

    bool Stream::AtEnd() const 
    { 
        return IsClosed() || (CanSeek() ? GetPosition() == GetLength() : false);
    }
    
    bool Stream::IsClosed() const 
    { 
        return false; 
    }

    bool Stream::CanRead() const 
    {
        return false; 
    }
    
    bool Stream::CanWrite() const 
    { 
        return false; 
    }
    
    bool Stream::CanSeek() const 
    { 
        return false; 
    }

    int Stream::Read(void* dst, int elmSize, int elmCount) 
    { 
        XLI_THROW_STREAM_CANT_READ; 
    }
    
    int Stream::Write(const void* src, int elmSize, int elmCount) 
    { 
        XLI_THROW_STREAM_CANT_WRITE;
    }

    void Stream::Seek(SeekOrigin origin, int offset) 
    { 
        XLI_THROW_STREAM_CANT_SEEK; 
    }
    
    int Stream::GetPosition() const 
    { 
        XLI_THROW_STREAM_CANT_SEEK; 
    }
    
    int Stream::GetLength() const 
    { 
        XLI_THROW_STREAM_CANT_SEEK; 
    }

    void Stream::ReadSafe(void* dst, int elmSize, int elmCount)
    {
        int r = Read(dst, elmSize, elmCount);

        if (r != elmCount)
        {
            if (AtEnd()) 
                XLI_THROW_END_OF_STREAM;
            
            XLI_THROW_STREAM_CANT_READ;
        }
    }

    void Stream::WriteSafe(const void* src, int elmSize, int elmCount)
    {
        if (Write(src, elmSize, elmCount) != elmCount)
        {
            if (AtEnd()) 
                XLI_THROW_END_OF_STREAM;
            
            XLI_THROW_STREAM_CANT_WRITE;
        }
    }

    void Stream::WriteStream(Stream* source)
    {
        // TODO: Use constant size buffer
        Managed<Buffer> buf = Buffer::Create(source->GetLength());
        source->Read(buf->Data(), 1, buf->Size());
        Write(buf->Data(), 1, buf->Size());
    }

    StreamWriter::StreamWriter(Stream* stream)
    {
        this->stream = 0;
        SwitchStream(stream);
    }

    StreamWriter::~StreamWriter()
    {
        stream->Release();
    }

    void StreamWriter::SwitchStream(Stream* stream)
    {
        if (!stream)
            XLI_THROW_NULL_POINTER;

        if (!stream->CanWrite())
            XLI_THROW_STREAM_CANT_WRITE;

        stream->AddRef();
        
        if (this->stream) 
            this->stream->Release();

        this->stream = stream;
    }

    Stream* StreamWriter::GetStream()
    {
        return stream;
    }

    StreamReader::StreamReader(Stream* stream)
    {
        this->stream = 0;
        SwitchStream(stream);
    }

    StreamReader::~StreamReader()
    {
        stream->Release();
    }

    void StreamReader::SwitchStream(Stream* stream)
    {
        if (!stream)
            XLI_THROW_NULL_POINTER;

        if (!stream->CanRead())
            XLI_THROW_STREAM_CANT_READ;

        stream->AddRef();
        
        if (this->stream) 
            this->stream->Release();
        
        this->stream = stream;
    }

    Stream* StreamReader::GetStream()
    {
        return stream;
    }
}
