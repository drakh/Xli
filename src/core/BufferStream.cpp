#include <Xli/BufferStream.h>

namespace Xli
{
	BufferStream::BufferStream(DataAccessor* buf, FileMode mode)
	{
		if (!buf) XLI_THROW_NULL_POINTER;
		
		this->buf = buf;
		this->buf->AddRef();
		this->pos = 0;
		this->read = (mode & FileModeRead) != 0;
		this->write = (mode & FileModeWrite) != 0;
	}

	BufferStream::~BufferStream()
	{
		buf->Release();
	}

	bool BufferStream::AtEnd() const
	{
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
		return buf->GetSizeInBytes();
	}

	int BufferStream::GetPosition() const
	{
		return pos;
	}

	int BufferStream::Read(void* data, int elementSize, int elementCount)
	{
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
		
		memcpy(data, buf->GetData() + pos, bytes);
		pos += bytes;

		return elementCount;
	}

	int BufferStream::Write(const void* data, int elementSize, int elementCount)
	{
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
		
		memcpy((UInt8*)buf->GetData() + pos, data, bytes);
		pos += bytes;

		return elementCount;
	}

	void BufferStream::Seek(SeekOrigin origin, int offset)
	{
		switch (origin)
		{
		case SeekOriginBegin:
			pos = offset;
			break;
		case SeekOriginCurrent:
			pos += offset;
			break;
		case SeekOriginEnd:
			pos = GetLength() + offset;
			break;
		}

		if (pos < 0) pos = 0;
		if (pos > GetLength()) pos = GetLength();
	}
}
