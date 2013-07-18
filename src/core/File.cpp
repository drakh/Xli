#include <Xli/File.h>
#include <Xli/TextReader.h>
#include <Xli/Unicode.h>

namespace Xli
{
	File::File(const String& filename, FileMode mode)
	{
		const char* m = "rb";
        
		switch (mode)
		{
            case FileModeRead: m = "rb"; break;
            case FileModeWrite: m = "wb"; break;
            case FileModeAppend: m = "ab"; break;
            case FileModeReadWrite: m = "r+b"; break;
            case FileModeReadWriteNew: m = "w+b"; break;
            case FileModeReadAppend: m = "a+b"; break;
            default: XLI_THROW(String("Invalid file mode: ") + FileModeToString(mode));
		}
        
#ifdef XLI_MSVC
		
		fp = 0;
		
		Utf16String filenameW = Unicode::Utf8To16(filename);
		Utf16String mW = Unicode::Utf8To16(m);
		
		if (_wfopen_s(&fp, filenameW.Data(), mW.Data()) != 0)
			XLI_THROW_CANT_OPEN_FILE(filename);

#else
		
		fp = fopen(filename.Data(), m);
		if (!fp) XLI_THROW_CANT_OPEN_FILE(filename);

#endif

		this->flags = FileFlagsCanClose | FileFlagsCanSeek;
        
		if (mode & FileModeRead)
			this->flags |= FileFlagsCanRead;

		if ((mode & FileModeWrite) || (mode & FileModeAppend))
			this->flags |= FileFlagsCanWrite;
	}
	
    File::File(FILE* fp, int flags)
	{
		this->fp = fp;
		this->flags = flags;
	}

	File::~File()
	{
		if (fp && (flags & FileFlagsCanClose)) 
			fclose(fp);
	}

	void File::Flush()
	{
		if (!fp) XLI_THROW_STREAM_CLOSED;
		fflush(fp);
	}

	void File::Close()
	{
		if (!fp) 
			XLI_THROW_STREAM_CLOSED;

		if (flags & FileFlagsCanClose)
			fclose(fp);

		fp = 0;
	}

	bool File::AtEnd() const
	{
		if (!fp) XLI_THROW_STREAM_CLOSED;
		return feof(fp) != 0;
	}

	bool File::IsClosed() const
	{
		return fp == 0;
	}

	bool File::CanRead() const
	{
		return fp != 0 && (flags & FileFlagsCanRead);
	}

	bool File::CanWrite() const
	{
		return fp != 0 && (flags & FileFlagsCanWrite);
	}

	bool File::CanSeek() const
	{
		return fp != 0 && (flags & FileFlagsCanSeek);
	}

	int File::GetPosition() const
	{
		if (!fp) XLI_THROW_STREAM_CLOSED;
		return (int)ftell(fp);
	}

	int File::GetLength() const
	{
		if (!fp) XLI_THROW_STREAM_CLOSED;
		long p = ftell(fp);
		fseek(fp, 0, SEEK_END);
		long l = ftell(fp);
		fseek(fp, p, SEEK_SET);
		return (int)l;
	}

	int File::Read(void* data, int elmSize, int elmCount)
	{
		if (!fp) XLI_THROW_STREAM_CLOSED;
		if (!(flags & FileFlagsCanRead)) XLI_THROW_STREAM_CANT_READ;
		return (int)fread(data, elmSize, elmCount, fp);
	}

	int File::Write(const void* data, int elmSize, int elmCount)
	{
		if (!fp) XLI_THROW_STREAM_CLOSED;
		if (!(flags & FileFlagsCanWrite)) XLI_THROW_STREAM_CANT_WRITE;
		return (int)fwrite(data, elmSize, elmCount, fp);
	}

	void File::Seek(SeekOrigin origin, int offset)
	{
		if (!fp) XLI_THROW_STREAM_CLOSED;
		if (!(flags & FileFlagsCanSeek)) XLI_THROW_STREAM_CANT_SEEK;

		switch (origin)
		{
		case SeekOriginBegin:
			if (fseek(fp, offset, SEEK_SET) == 0) return;
			break;

		case SeekOriginCurrent:
			if (fseek(fp, offset, SEEK_CUR) == 0) return;
			break;

		case SeekOriginEnd:
			if (fseek(fp, offset, SEEK_END) == 0) return;
			break;
		}

		XLI_THROW_STREAM_CANT_SEEK;
	}
}
