#include <XliMedia/Zip.h>
#include <Xli/BufferStream.h>
#include "3rdparty/unzip/unzip.h"

namespace Xli
{
	class ZipFileSystem: public FileSystem
	{
		Shared<Stream> file;
		zlib_filefunc_def pzlib_filefunc_def;
		unzFile handle;

	public:
		static voidpf ZCALLBACK open_file_func(voidpf opaque, const char* filename, int mode)
		{
			return opaque;
		}
		static uLong ZCALLBACK read_file_func(voidpf opaque, voidpf stream, void* buf, uLong size)
		{
			return (uLong)((Stream*)stream)->Read(buf, size, 1);
		}
		static uLong ZCALLBACK write_file_func(voidpf opaque, voidpf stream, const void* buf, uLong size)
		{
			return (uLong)((Stream*)stream)->Write((void*)buf, size, 1);
		}
		static long ZCALLBACK tell_file_func(voidpf opaque, voidpf stream)
		{
			return (long)((Stream*)stream)->GetPosition();
		}
		static long ZCALLBACK seek_file_func(voidpf opaque, voidpf stream, uLong offset, int origin)
		{
			switch (origin)
			{
			case ZLIB_FILEFUNC_SEEK_CUR: ((Stream*)stream)->Seek(SeekOriginCurrent, (int)offset); return 0;
			case ZLIB_FILEFUNC_SEEK_END: ((Stream*)stream)->Seek(SeekOriginEnd, (int)offset); return 0;
			case ZLIB_FILEFUNC_SEEK_SET: ((Stream*)stream)->Seek(SeekOriginBegin, (int)offset); return 0;
			default: return -1L;
			}
		}
		static int ZCALLBACK close_file_func(voidpf opaque, voidpf stream)
		{
			return 0;
		}
		static int ZCALLBACK error_file_func(voidpf opaque, voidpf stream)
		{
			return -1;
		}

		ZipFileSystem(Stream* file, const char* password)
		{
			int fpos = file->GetPosition();
			this->file = file;

			pzlib_filefunc_def.zopen_file = open_file_func;
			pzlib_filefunc_def.zread_file = read_file_func;
			pzlib_filefunc_def.zwrite_file = write_file_func;
			pzlib_filefunc_def.ztell_file = tell_file_func;
			pzlib_filefunc_def.zseek_file = seek_file_func;
			pzlib_filefunc_def.zclose_file = close_file_func;
			pzlib_filefunc_def.zerror_file = error_file_func;
			pzlib_filefunc_def.opaque = (voidpf)this->file.Get();

			handle = unzOpen2(0, &pzlib_filefunc_def);

			if (!handle)
			{
				file->Seek(SeekOriginBegin, fpos);
				XLI_THROW("Could not open Zip-archive");
			}

			if (password)
			{
				if (unzOpenCurrentFilePassword(handle, password) != UNZ_OK)
				{
					XLI_THROW("Wrong zip password");
				}
			}
		}

		~ZipFileSystem()
		{
			unzClose(handle);
		}

		virtual Stream* OpenFile(const String& filename, FileMode fileMode)
		{
			if (fileMode != FileModeRead) XLI_THROW("Invalid FileMode; Zip is Read-Only");

			if (unzLocateFile((unzFile)handle, filename.Data(), 2) != UNZ_OK) XLI_THROW(String("Couldn't locate file in zip-archive: ") + filename);
			if (unzOpenCurrentFile((unzFile)handle) != UNZ_OK) XLI_THROW(String("Couldn't open file in zip-archive: ") + filename);

			unz_file_info unzFileInfo;
			unzGetCurrentFileInfo((unzFile)handle, &unzFileInfo, 0, 0, 0, 0, 0, 0);

			Managed<Buffer> buf = Buffer::Create(unzFileInfo.uncompressed_size);

			if (unzReadCurrentFile((unzFile)handle, buf->Data(), buf->Size()) != buf->Size()) XLI_THROW(String("Couldn't read in from zip-archive: ") + filename);
			if (unzCloseCurrentFile((unzFile)handle) != UNZ_OK ) XLI_THROW(String("Couldn't close file in zip-archive: ") + filename);

			return new BufferStream(buf, FileModeRead);
		}

		virtual void GetFiles(const String& path, Array<FileInfo>& list)
		{
			XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
		}
	};

	FileSystem* Zip::Open(Stream* file, const char* password)
	{
		return new ZipFileSystem(file, password);
	}
}