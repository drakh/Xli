#include <Xli/Stream.h>
#include <Xli/Console.h>

namespace Xli
{
	Stream::~Stream() {}

	void Stream::Flush() {}
	void Stream::Close() {}
	bool Stream::AtEnd() const { return CanSeek() ? GetPosition() == GetLength() : false; }
	bool Stream::IsClosed() const { return false; }

	bool Stream::CanRead() const { return false; }
	bool Stream::CanWrite() const { return false; }
	bool Stream::CanSeek() const { return false; }

	int Stream::Read(void* dst, int elmSize, int elmCount) { XLI_THROW_STREAM_CANT_READ; }
	int Stream::Write(const void* src, int elmSize, int elmCount) { XLI_THROW_STREAM_CANT_WRITE; }
	void Stream::Seek(SeekOrigin origin, int offset) { XLI_THROW_STREAM_CANT_SEEK; }

	int Stream::GetPosition() const { XLI_THROW_STREAM_CANT_SEEK; }
	int Stream::GetLength() const { XLI_THROW_STREAM_CANT_SEEK; }

	void Stream::ReadSafe(void* dst, int elmSize, int elmCount)
	{
		int r = Read(dst, elmSize, elmCount);
		if (r != elmCount)
		{
			if (AtEnd()) XLI_THROW_END_OF_STREAM;
			XLI_THROW_STREAM_CANT_READ;
		}
	}

	void Stream::WriteSafe(const void* src, int elmSize, int elmCount)
	{
		if (Write(src, elmSize, elmCount) != elmCount)
		{
			if (AtEnd()) XLI_THROW_END_OF_STREAM;
			XLI_THROW_STREAM_CANT_WRITE;
		}
	}

	void Stream::WriteStream(Stream* source)
	{
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
		{
			XLI_THROW_NULL_POINTER;
		}
		if (!stream->CanWrite())
		{
			XLI_THROW_STREAM_CANT_WRITE;
		}

		stream->AddRef();
		if (this->stream) this->stream->Release();
		this->stream = stream;
	}
	Stream* StreamWriter::GetStream()
	{
		return stream;
	}

	StreamReader::StreamReader(Stream* stream): stream(stream)
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
		{
			XLI_THROW_NULL_POINTER;
		}
		if (!stream->CanRead())
		{
			XLI_THROW_STREAM_CANT_READ;
		}

		stream->AddRef();
		if (this->stream) this->stream->Release();
		this->stream = stream;
	}
	Stream* StreamReader::GetStream()
	{
		return stream;
	}

	StaticStream::StaticStream(DataAccessor* buf)
	{
		if (!buf) XLI_THROW_NULL_POINTER;
		this->buf = buf;
		this->buf->AddRef();
		pos = 0;
	}

	StaticStream::~StaticStream()
	{
		buf->Release();
	}

	bool StaticStream::AtEnd() const
	{
		return pos == buf->GetSizeInBytes();
	}

	bool StaticStream::CanRead() const
	{
		return true;
	}

	bool StaticStream::CanSeek() const
	{
		return true;
	}

	int StaticStream::GetLength() const
	{
		return buf->GetSizeInBytes();
	}

	int StaticStream::GetPosition() const
	{
		return pos;
	}

	int StaticStream::Read(void* data, int elementSize, int elementCount)
	{
		int bytes = elementCount * elementSize;

		if (bytes > GetLength() - pos)
		{
			bytes = GetLength() - pos;
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

	void StaticStream::Seek(SeekOrigin origin, int offset)
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


	DynamicStream::DynamicStream(int initCapacity)
	{
		buf.Reserve(initCapacity);
	}

	bool DynamicStream::CanWrite() const
	{
		return true;
	}

	int DynamicStream::Write(const void* src, int elmSize, int elmCount)
	{
		int loc = buf.Length();
		buf.Expand();
		buf.Resize(buf.Length()+elmSize*elmCount);
		memcpy(buf.Data()+loc, src, elmSize*elmCount);
		return elmCount;
	}

	int DynamicStream::GetLength() const
	{
		return buf.Length();
	}

	const UInt8* DynamicStream::GetData() const
	{
		return buf.Data();
	}

	Buffer* DynamicStream::CopyData() const
	{
		return Buffer::Copy(buf.Data(), buf.Length());
	}

	void DynamicStream::Clear()
	{
		buf.Clear();
	}

}
