#include <Xli/NativeFileSystem.h>
#include <cstdlib>

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

	void NativeFileSystem::Done()
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

	void AssetFileSystem::Done()
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
			atexit(NativeFileSystem::Done);
		}

		return fs;
	}

	DiskAccessor::operator FileSystem*()
	{
		if (!fs)
		{
			NativeFileSystem::Init();
			atexit(NativeFileSystem::Done);
		}

		return fs;
	}

	DiskAccessor Disk;

	void AssetsAccessor::SetFilesystem(FileSystem* fs)
	{
		if (!as) 
			atexit(AssetFileSystem::Done);
		else 
			as->Release();

		fs->AddRef();
		as = fs;
	}

	FileSystem* AssetsAccessor::operator ->()
	{
		if (!as)
		{
			AssetFileSystem::Init();
			atexit(AssetFileSystem::Done);
		}

		return as;
	}

	AssetsAccessor::operator FileSystem*()
	{
		if (!as)
		{
			AssetFileSystem::Init();
			atexit(AssetFileSystem::Done);
		}

		return as;
	}

	AssetsAccessor Assets;
}
