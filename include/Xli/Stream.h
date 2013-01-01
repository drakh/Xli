#ifndef __XLI_STREAM_H__
#define __XLI_STREAM_H__

#include <Xli/BaseTypes.h>
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
		\ingroup IO
	*/
	enum SeekOrigin
	{
		SeekOriginBegin,
		SeekOriginCurrent,
		SeekOriginEnd
	};

	/**
		\ingroup IO
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

		virtual int BytesWaiting() { return 0; }

		virtual int Read(void* dst, int elmSize, int elmCount);
		virtual int Write(const void* src, int elmSize, int elmCount);
		virtual void Seek(SeekOrigin origin, int offset);

		void ReadSafe(void* dst, int elmSize, int elmCount);

		void WriteSafe(const void* src, int elmSize, int elmCount);

		/**
			Write the entire contents of another stream into this stream.
			@param source Stream to read from.
		*/
		void WriteStream(Stream* source);
        
    
        virtual DataAccessor* CreateDataAccessor()
        {
            Buffer* buf = Buffer::Create((unsigned int)GetLength());
            ReadSafe(buf->Data(), 1, buf->Size());
            return buf;
        }
	};

	/**
		\ingroup IO
	*/
	class StreamWriter: public Object
	{
	protected:
		Stream* stream;

	public:
		StreamWriter(Stream* stream);
		virtual ~StreamWriter();

		void SwitchStream(Stream* stream);
		Stream* GetStream();
	};

	/**
		\ingroup IO
	*/
	class StreamReader: public Object
	{
	protected:
		Stream* stream;

	public:
		StreamReader(Stream* stream);
		virtual ~StreamReader();
		
		void SwitchStream(Stream* stream);
		Stream* GetStream();
	};

	/**
		\ingroup IO
	*/
	class StaticStream: public Stream
	{
		DataAccessor* buf;
		int pos;

	public:
		StaticStream(DataAccessor* buf);
		virtual ~StaticStream();

		virtual bool AtEnd() const;

		virtual bool CanRead() const;
		virtual bool CanSeek() const;

		virtual int Read(void* dst, int elmSize, int elmCount);
		virtual void Seek(SeekOrigin origin, int offset);

		virtual int GetPosition() const;
		virtual int GetLength() const;
	};

	/**
		\ingroup IO
	*/
	class DynamicStream: public Stream
	{
	protected:
		Array<UInt8> buf;

	public:
		DynamicStream(int initCapacity = 1024);

		virtual bool CanWrite() const;
		virtual int Write(const void* src, int elmSize, int elmCount);

		virtual int GetLength() const;

		const UInt8* GetData() const;
		Buffer* CopyData() const;

		void Clear();
	};

	/**
		\ingroup IO
	*/
	class TextStream: public DynamicStream
	{
	public:
		TextStream(int initCapacity = 1024): DynamicStream(initCapacity) {}
		CharString GetText() const { return CharString((char*)buf.Data(), GetLength()); }
	};
}

#endif
