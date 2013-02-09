#include <XliMedia/Gzip.h>
#include <Xli/Console.h>
#include <Xli/BufferStream.h>
#include <cstring>
#include <zlib.h>

#define DEF_MEM_LEVEL 8
#define Z_BUFSIZE 16384

static int const gz_magic[2] = {0x1f, 0x8b}; /* gzip magic header */

/* gzip flag byte */
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */

namespace Xli
{
	// TODO: Writer is broken
	class GzStreamWriter : public Stream
	{
		Shared<Stream> compressedStream;
		Bytef buf[Z_BUFSIZE];
		z_stream stream;
		uLong crc;
		int pos;

	public:
		GzStreamWriter(Stream* targetStream, int level)
		{
			if (targetStream == 0) XLI_THROW_NULL_POINTER;

			this->crc = 0;
			this->pos = 0;
			this->compressedStream = targetStream;

			level = Clamp(level, Z_BEST_SPEED, Z_BEST_COMPRESSION);

			memset(&stream, 0, sizeof(z_stream));
			stream.next_out = buf;
			stream.avail_out = Z_BUFSIZE;

			int err = deflateInit2(&stream, level, Z_DEFLATED, 15 + 16, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);

			if (err != Z_OK || !buf) XLI_THROW("Failed to create gzip stream");

#if 0
			char header[11];

			sprintf_s(header, "%c%c%c%c%c%c%c%c%c%c", gz_magic[0], gz_magic[1],
				Z_DEFLATED, 0 /*flags*/, 0,0,0,0 /*time*/, 0 /*xflags*/, OS_CODE);

			compressedStream->WriteSafe(header, 1, 10);
#endif
		}

		virtual ~GzStreamWriter()
		{
			Close();
		}

		virtual void Flush()
		{
			if (compressedStream.IsNull()) XLI_THROW_STREAM_CLOSED;

			while (true)
			{
				int len = Z_BUFSIZE - stream.avail_out;
				if (len > 0) compressedStream->WriteSafe(buf, 1, len);

				stream.next_out = buf;
				stream.avail_out = Z_BUFSIZE;

				int err = deflate(&stream, Z_FINISH);

				if (err == Z_OK)
				{
					continue;
				}
				if (err != Z_STREAM_END)
				{
					XLI_THROW("Failed to flush gzip stream");
				}

				break;
			}
		}

		virtual void Close()
		{
			if (compressedStream.IsNull()) return;
			
			Flush();
			//compressedStream->WriteSafe(&crc, 4, 1);
			//compressedStream->WriteSafe(&pos, 4, 1);
			compressedStream->Close();
			compressedStream = 0;
		}

		virtual bool CanWrite() const
		{
			return compressedStream.IsSet();
		}

		virtual int GetPosition() const
		{
			return pos;
		}

		virtual int GetLength() const
		{
			return pos;
		}

		virtual int Write(const void* data, int elmSize, int elmCount)
		{
			if (compressedStream.IsNull()) XLI_THROW_STREAM_CLOSED;

			int len = elmSize * elmCount;

			stream.next_in = (Bytef*)data;
			stream.avail_in = len;

			while (stream.avail_in != 0)
			{
				if (stream.avail_out == 0)
				{
					stream.next_out = buf;
					stream.avail_out = Z_BUFSIZE;
					compressedStream->WriteSafe(buf, 1, Z_BUFSIZE);
				}

				int err = deflate(&stream, Z_NO_FLUSH);
				
				if (err != Z_OK)
				{
					XLI_THROW("Failed to write to gzip stream");
				}
			}
			
			pos += len;
			crc = crc32(crc, (const Bytef*)data, len);

			return elmCount;
		}
	};
	/*
	Stream* Gzip::CreateWriter(Stream* targetStream, int level)
	{
		return new GzStreamWriter(targetStream, level);
	}
	*/
	Stream* Gzip::CreateReader(Stream* sourceStream)
	{
		if (sourceStream == 0) XLI_THROW_NULL_POINTER;

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

		int err = inflateInit2(&stream, 16 + MAX_WBITS);

		stream.next_in = src->Data();
		stream.avail_in = src->Size();
		stream.next_out = dst->Data();
		stream.avail_out = dst->Size();

		if (err != Z_OK) XLI_THROW("Failed to create gzip stream");

		while (true)
		{
			int result = inflate(&stream, Z_FINISH);

			if (result == Z_OK)
			{
				continue;
			}
			else if (result == Z_STREAM_END)
			{
				break;
			}
			else if (result == Z_DATA_ERROR)
			{
				XLI_THROW("Failed to create gzip stream: Data error");
			}
			else
			{
				XLI_THROW("Failed to create gzip stream: Unknown error");
			}
		}

		if (stream.avail_out != 0 || stream.avail_in != 0)
		{
			ErrorPrintLine("GZIP WARNING: Buffer sizes inconsistent");
		}

		return new BufferStream(dst, true, false);
	}
}
