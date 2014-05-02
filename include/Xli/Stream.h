#ifndef __XLI_STREAM_H__
#define __XLI_STREAM_H__

#include <Xli/IntTypes.h>
#include <Xli/Object.h>
#include <Xli/String.h>
#include <Xli/Buffer.h>
#include <Xli/Vector2.h>
#include <Xli/Vector3.h>
#include <Xli/Vector4.h>
#include <Xli/Array.h>
#include <Xli/Managed.h>

namespace Xli
{
    /**
        \ingroup XliCoreIO
    */
    enum SeekOrigin
    {
        SeekOriginBegin,
        SeekOriginCurrent,
        SeekOriginEnd
    };

    /**
        \ingroup XliCoreIO
    */
    class Stream: public Object
    {
    public:
        virtual ~Stream();

        virtual void Flush();
        virtual void Close();

        virtual bool IsClosed() const;
        virtual bool AtEnd() const;

        virtual bool CanRead() const;
        virtual bool CanWrite() const;
        virtual bool CanSeek() const;

        virtual int GetPosition() const;
        virtual int GetLength() const;

        virtual int Read(void* dst, int elmSize, int elmCount);
        virtual void Write(const void* src, int elmSize, int elmCount);
        virtual void Seek(int offset, SeekOrigin origin);

        void ReadSafe(void* dst, int elmSize, int elmCount);

        /**
            Write the entire contents of another stream into this stream.
            @param source Stream to read from.
        */
        void WriteStream(Stream* source);
    
        virtual DataAccessor* CreateDataAccessor()
        {
            Buffer* buf = Buffer::Create(GetLength());
            ReadSafe(buf->DataPtr(), 1, buf->Size());
            return buf;
        }
    };

    /**
        \ingroup XliCoreIO
    */
    class StreamWriterBase: public Object
    {
    protected:
        Stream* _stream;

    public:
        StreamWriterBase(Stream* stream);
        virtual ~StreamWriterBase();

        void SetStream(Stream* stream);
        Stream* GetStream();
    };

    /**
        \ingroup XliCoreIO
    */
    class StreamReaderBase: public Object
    {
    protected:
        Stream* _stream;

    public:
        StreamReaderBase(Stream* stream);
        virtual ~StreamReaderBase();
        
        void SetStream(Stream* stream);
        Stream* GetStream();
    };
}

#endif
