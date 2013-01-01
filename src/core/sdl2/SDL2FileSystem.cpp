#include "SDL2FileSystem.h"
#include <Xli/Console.h>
#include <Xli/Unicode.h>
#include <SDL.h>

namespace Xli
{
	class SDL2File : public Stream
	{
		SDL_RWops* fp;
		bool canRead, canWrite;

	public:
		SDL2File(const String& fileName, FileMode mode)
		{
			fp = 0;
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

			CharString utf8Filename = Unicode::Utf16To8(fileName);

			// This will look inside the app bundle on MacOSX and iOS
			fp = SDL_RWFromFile(utf8Filename.Data(), m); 

			if (!fp)
			{
				XLI_THROW_CANT_OPEN_FILE(fileName);
			}

			this->canRead = (mode & FileModeRead) != 0;
			this->canWrite = (mode & FileModeWrite) != 0 || (mode & FileModeAppend) != 0;
		}

		virtual ~SDL2File()
		{
			if (fp) SDL_RWclose(fp);
		}

		void Flush()
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
		}

		void Close()
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			if (SDL_RWclose(fp) != 0) ErrorPrintLine("WARNING: Failed to close file stream");
			fp = 0;
		}

		bool AtEnd() const
		{
			return GetPosition() == GetLength();
		}

		bool IsClosed() const
		{
			return fp == 0;
		}

		bool CanRead() const
		{
			return canRead && fp != 0;
		}
		bool CanWrite() const
		{
			return canWrite && fp != 0;
		}
		bool CanSeek() const
		{
			return fp != 0;
		}

		int GetPosition() const
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			return (int)SDL_RWtell(fp);
		}
		int GetLength() const
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			long p = SDL_RWtell(fp);
			SDL_RWseek(fp, 0, SEEK_END);
			long l = SDL_RWtell(fp);
			SDL_RWseek(fp, p, SEEK_SET);
			return (int)l;
		}

		int Read(void* data, int elmSize, int elmCount)
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			if (!canRead) XLI_THROW_STREAM_CANT_READ;
			return SDL_RWread(fp, data, elmSize, elmCount);
		}
		int Write(const void* data, int elmSize, int elmCount)
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			if (!canWrite) XLI_THROW_STREAM_CANT_WRITE;
			return SDL_RWwrite(fp, data, elmSize, elmCount);
		}
		void Seek(SeekOrigin origin, int offset)
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;

			switch (origin)
			{
			case SeekOriginBegin:
				if (SDL_RWseek(fp, offset, SEEK_SET) >= 0) return;
				break;
			case SeekOriginCurrent:
				if (SDL_RWseek(fp, offset, SEEK_CUR) >= 0) return;
				break;
			case SeekOriginEnd:
				if (SDL_RWseek(fp, offset, SEEK_END) >= 0) return;
				break;
			}

			XLI_THROW_STREAM_CANT_SEEK;
		}
	};

    Stream* SDL2FileSystem::OpenFile(const String& fileName, FileMode mode)
    {
        return new SDL2File(fileName, mode);
    }

	NativeFileSystem* CreateNativeFileSystem()
	{
		return new SDL2FileSystem();
	}
}
