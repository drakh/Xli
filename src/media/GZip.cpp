#include <XliMedia/GZip.h>
#include <Xli/Console.h>
#include <Xli/BufferStream.h>
#include <cstring>
#include <zlib.h>

#define DEF_MEM_LEVEL 8
#define Z_BUFSIZE 0x4000

namespace Xli
{
    class GZipWriter: public Stream
    {
        Shared<Stream> dst;
        Bytef buf[Z_BUFSIZE];
        z_stream stream;
        int pos;

    public:
        GZipWriter(Stream* targetStream, int level, bool header)
        {
            if (!targetStream) 
                XLI_THROW_NULL_POINTER;

            this->pos = 0;
            this->dst = targetStream;

            memset(&stream, 0, sizeof(z_stream));

            int windowBits = header ? MAX_WBITS + 16 : -MAX_WBITS;

            if (deflateInit2(&stream, level, Z_DEFLATED, windowBits, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK) 
                XLI_THROW("Failed to create GZip stream");

            stream.next_out = buf;
            stream.avail_out = Z_BUFSIZE;
        }

        virtual ~GZipWriter()
        {
            Close();
        }

        virtual void Flush()
        {
            if (!dst) 
                XLI_THROW_STREAM_CLOSED;

            // TODO: Flush gzip buffer

            dst->Flush();
        }

        virtual void Close()
        {
            if (!dst) 
                return;

            while (true)
            {
                int err = deflate(&stream, Z_FINISH);

                if (err == Z_STREAM_END)
                    break;
                else if (err != Z_OK || stream.avail_out != 0)
                    XLI_THROW("Failed to finish GZip writer");

                stream.next_out = buf;
                stream.avail_out = Z_BUFSIZE;
                dst->WriteSafe(buf, 1, Z_BUFSIZE);
            }
            
            if (deflateEnd(&stream) != Z_OK)
                XLI_THROW("Failed to close GZip writer");

            dst->WriteSafe(buf, 1, Z_BUFSIZE - stream.avail_out);
            dst = 0;
        }

        virtual bool IsClosed() const
        {
            return !dst;
        }

        virtual bool CanWrite() const
        {
            return true;
        }

        virtual int GetLength() const
        {
            return pos;
        }

        virtual int Write(const void* data, int elmSize, int elmCount)
        {
            if (!dst) 
                XLI_THROW_STREAM_CLOSED;

            int len = elmSize * elmCount;

            stream.next_in = (Bytef*)data;
            stream.avail_in = len;

            pos += len;

            do
            {
                if (deflate(&stream, Z_NO_FLUSH) != Z_OK)
                    XLI_THROW("Failed to write to GZip stream");

                if (!stream.avail_out)
                {
                    stream.next_out = buf;
                    stream.avail_out = Z_BUFSIZE;
                    dst->WriteSafe(buf, 1, Z_BUFSIZE);
                }
            } 
            while (stream.avail_in > 0);
            
            return elmCount;
        }
    };
/*
    class GZipReader: public Stream
    {

    };
*/    
    Stream* GZip::CreateWriter(Stream* targetStream, int level, bool header)
    {
        return new GZipWriter(targetStream, level, header);
    }
    
    Stream* GZip::CreateReader(Stream* sourceStream)
    {
        if (sourceStream == 0) 
            XLI_THROW_NULL_POINTER;

        int sp = sourceStream->GetPosition();
        int sl = sourceStream->GetLength();

        uLong expected_crc;
        int expected_len;

        sourceStream->Seek(SeekOriginEnd, -8);
        sourceStream->ReadSafe(&expected_crc, 4, 1);
        sourceStream->ReadSafe(&expected_len, 4, 1);
        sourceStream->Seek(SeekOriginBegin, sp);

        int input_size = sl - sp;

        Managed<Buffer> src = Buffer::Create(input_size);
        Managed<Buffer> dst = Buffer::Create(expected_len);

        sourceStream->ReadSafe(src->Data(), 1, src->Size());

        z_stream stream;
        memset(&stream, 0, sizeof(z_stream));

        stream.next_in = src->Data();
        stream.avail_in = src->Size();
        stream.next_out = dst->Data();
        stream.avail_out = dst->Size();

        int err = inflateInit2(&stream, 16 + MAX_WBITS);

        if (err != Z_OK) 
            XLI_THROW("Failed to create GZip stream");

        while (true)
        {
            err = inflate(&stream, Z_FINISH);

            if (err == Z_OK)
                continue;
            else if (err == Z_STREAM_END)
                break;
            else if (err == Z_DATA_ERROR)
                XLI_THROW("Failed to create GZip stream: Data error");
            else
                XLI_THROW("Failed to create GZip stream");
        }

        if (stream.avail_out != 0 || stream.avail_in != 0)
            ErrorPrintLine("GZIP WARNING: Buffer sizes inconsistent");

        return new BufferStream(dst, true, false);
    }
}
