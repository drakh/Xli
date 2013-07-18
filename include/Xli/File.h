#ifndef __XLI_FILE_H__
#define __XLI_FILE_H__

#include <Xli/FileSystem.h>
#include <cstdio>

namespace Xli
{
	/**
		\ingroup IO
	*/
	enum FileFlags
	{
		FileFlagsCanRead = 1 << 0,
		FileFlagsCanWrite = 1 << 1,
		FileFlagsCanSeek = 1 << 2,
		FileFlagsCanClose = 1 << 3,
	};

	/**
		\ingroup IO
	*/
	class File: public Stream
	{
		FILE* fp;
		int flags;
		
	public:
		File(const String& filename, FileMode mode);
		File(FILE* fp, int flags);
		virtual ~File();

		virtual void Flush();
		virtual void Close();

		virtual bool AtEnd() const;
		virtual bool IsClosed() const;

		virtual bool CanRead() const;
		virtual bool CanWrite() const;
		virtual bool CanSeek() const;

		virtual int GetPosition() const;
		virtual int GetLength() const;

		virtual int Read(void* data, int elmSize, int elmCount);
		virtual int Write(const void* data, int elmSize, int elmCount);
		virtual void Seek(SeekOrigin origin, int offset);
	};
}

#endif
