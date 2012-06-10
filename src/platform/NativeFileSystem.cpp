#include <XliPlatform/NativeFileSystem.h>

namespace Xli
{
	NativeFileSystem* CreateNativeFileSystem();
	FileSystem* CreateAssetFileSystem();

	static NativeFileSystem* fs = 0;
	static FileSystem* as = 0;

	void NativeFileSystem::Init()
	{
		if (!fs)
		{
			fs = CreateNativeFileSystem();
		}
		else
		{
			fs->AddRef();
		}
	}

	void NativeFileSystem::Shutdown()
	{
		if (fs && fs->GetRefCount() > 1)
		{
			fs->Release();
		}
		else
		{
			fs->Release();
			fs = 0;
		}
	}

	void AssetFileSystem::Init()
	{
		if (!as)
		{
			as = CreateAssetFileSystem();
		}
		else
		{
			as->AddRef();
		}
	}

	void AssetFileSystem::Shutdown()
	{
		if (as && as->GetRefCount() > 1)
		{
			as->Release();
		}
		else
		{
			as->Release();
			as = 0;
		}
	}

	NativeFileSystem* DiskAccessor::operator ->()
	{
		if (!fs)
		{
			NativeFileSystem::Init();
			atexit(NativeFileSystem::Shutdown);
		}
		return fs;
	}

	DiskAccessor::operator FileSystem*()
	{
		if (!fs)
		{
			NativeFileSystem::Init();
			atexit(NativeFileSystem::Shutdown);
		}
		return fs;
	}

	DiskAccessor Disk;

	void AssetAccessor::SetFilesystem(FileSystem* fs)
	{
		if (!as) atexit(AssetFileSystem::Shutdown);
		else as->Release();
		fs->AddRef();
		as = fs;
	}

	FileSystem* AssetAccessor::operator ->()
	{
		if (!as)
		{
			AssetFileSystem::Init();
			atexit(AssetFileSystem::Shutdown);
		}
		return as;
	}

	AssetAccessor::operator FileSystem*()
	{
		if (!as)
		{
			AssetFileSystem::Init();
			atexit(AssetFileSystem::Shutdown);
		}
		return as;
	}

	AssetAccessor Assets;
}
