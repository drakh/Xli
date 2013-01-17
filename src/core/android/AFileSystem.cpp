#include <Xli/NativeFileSystem.h>
#include "../posix/PosixFileSystemBase.h"

#include <android/asset_manager.h>
extern struct AAssetManager* XliAAssetManager;

#include <cstdio>

namespace Xli
{
	class AAssetStream: public Stream
	{
		AAsset* asset;

	public:
		AAssetStream(String filename, FileMode mode)
		{
			if (mode != FileModeRead && mode != FileModeReadRandom) XLI_THROW("Unsupported asset file mode: " + (String)FileModeToString(mode));
			asset = AAssetManager_open(XliAAssetManager, filename.Data(), ((mode & FileModeRandom) != 0) ? AASSET_MODE_RANDOM : AASSET_MODE_STREAMING);
			if (asset == 0) XLI_THROW_CANT_OPEN_FILE(filename);
		}
		virtual ~AAssetStream()
		{
			AAsset_close(asset);
		}
		virtual bool CanRead() const
		{
			return true;
		}
		virtual bool CanSeek() const
		{
			return true;
		}
		int GetPosition() const
		{
			return (int)(AAsset_getLength(asset) - AAsset_getRemainingLength(asset));
		}
		int GetLength() const
		{
			return AAsset_getLength(asset);
		}
		int Read(void* data, int elmSize, int elmCount)
		{
			return AAsset_read(asset, data, elmSize * elmCount) / elmSize;
		}
		void Seek(SeekOrigin origin, int offset)
		{
			switch (origin)
			{
			case SeekOriginBegin:
				AAsset_seek(asset, offset, SEEK_SET);
				return;
			case SeekOriginCurrent:
				AAsset_seek(asset, offset, SEEK_CUR);
				return;
			case SeekOriginEnd:
				AAsset_seek(asset, offset, SEEK_END);
				return;
			}

			XLI_THROW_STREAM_CANT_SEEK;
		}
	};

	class AAssetBuffer: public DataAccessor
	{
		AAsset* asset;

	public:
		AAssetBuffer(String filename)
		{
			asset = AAssetManager_open(XliAAssetManager, filename.Data(), AASSET_MODE_BUFFER);
			if (asset == 0) XLI_THROW_CANT_OPEN_FILE(filename);
		}
		virtual ~AAssetBuffer()
		{
			AAsset_close(asset);
		}
		virtual const UInt8* GetData() const
		{
			return (const UInt8*)AAsset_getBuffer(asset);
		}
		virtual int GetSizeInBytes() const
		{
			return AAsset_getLength(asset);
		}
	};

	class AAssetFileSystem: public FileSystem
	{
	public:
		virtual Stream* OpenFile(const String& filename, FileMode mode)
		{
			return new AAssetStream(filename, mode);
		}
		virtual DataAccessor* OpenFileAsBuffer(const String& filename)
		{
			return new AAssetBuffer(filename);
		}
	};

	FileSystem* CreateAssetFileSystem()
	{
		return new AAssetFileSystem();
	}

	class APosixFileSystem: public PosixFileSystemBase
	{
	public:
		virtual String GetSystemDirectory()
		{
			// TODO: Conform to Android specifications on proper handling of shared document files
			return "/sdcard";
		}
	};

	NativeFileSystem* CreateNativeFileSystem()
	{
		return new APosixFileSystem();
	}
}
