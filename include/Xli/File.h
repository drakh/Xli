#ifndef __XLI_FILE_H__
#define __XLI_FILE_H__

#include <Xli/FileSystem.h>
#include <cstdio>

namespace Xli
{
	class File: public Stream
	{
		FILE* fp;
		bool canRead, canWrite;
		
	public:
		File(const String& fileName, FileMode mode);
		File(FILE* fp, bool canRead, bool canWrite);
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
