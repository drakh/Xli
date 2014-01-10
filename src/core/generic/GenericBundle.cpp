#include <Xli/Disk.h>

namespace Xli
{
    FileSystem* CreateBundleFileSystem()
    {
        return Disk->CreateSubFileSystem("data");
    }
}
