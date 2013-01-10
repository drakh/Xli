#ifndef __XLI_ARRAY_STREAM_H__
#define __XLI_ARRAY_STREAM_H__

#include <Xli/File.h>

namespace Xli
{
	/**
		\ingroup IO
	*/
	class BufferStream: public Stream
	{
		DataAccessor* buf;
		int pos;
		bool read, write;

	public:
		BufferStream(DataAccessor* buf, FileMode mode);
		virtual ~BufferStream();

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
