#ifndef __XLI_BUFFER_STREAM_H__
#define __XLI_BUFFER_STREAM_H__

#include <Xli/Stream.h>

namespace Xli
{
	/**
		\ingroup XliCoreIO
	*/
	class BufferStream: public Stream
	{
		DataAccessor* buf;
		bool read, write, closed;
		int pos;

	public:
		BufferStream(DataAccessor* buf, bool canRead, bool canWrite);
		virtual ~BufferStream();

		virtual void Close();

		virtual bool IsClosed() const;
		virtual bool AtEnd() const;

		virtual bool CanRead() const;
		virtual bool CanWrite() const;
		virtual bool CanSeek() const;

		virtual int Read(void* dst, int elmSize, int elmCount);
		virtual int Write(const void* dst, int elmSize, int elmCount);
		virtual void Seek(SeekOrigin origin, int offset);

		virtual int GetPosition() const;
		virtual int GetLength() const;
	};
}

#endif
