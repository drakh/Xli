#include <XliPlatform/Disk.h>
#include <cstdlib>

namespace Xli
{
    NativeFileSystem* CreateNativeFileSystem();

    static NativeFileSystem* fs = 0;

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
}
