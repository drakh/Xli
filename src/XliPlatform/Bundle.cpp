#include <XliPlatform/Bundle.h>
#include <cstdlib>

namespace Xli
{
    FileSystem* CreateBundleFileSystem();

    static FileSystem* as = 0;

    void BundleFileSystem::Init()
    {
        if (!as)
        {
            as = CreateBundleFileSystem();
        }
        else
        {
            as->AddRef();
        }
    }

    void BundleFileSystem::Done()
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

    void BundleAccessor::SetFilesystem(FileSystem* fs)
    {
        if (!as)
            atexit(BundleFileSystem::Done);
        else
            as->Release();

        fs->AddRef();
        as = fs;
    }

    FileSystem* BundleAccessor::operator ->()
    {
        if (!as)
        {
            BundleFileSystem::Init();
            atexit(BundleFileSystem::Done);
        }

        return as;
    }

    BundleAccessor::operator FileSystem*()
    {
        if (!as)
        {
            BundleFileSystem::Init();
            atexit(BundleFileSystem::Done);
        }

        return as;
    }

    BundleAccessor Bundle;
}
