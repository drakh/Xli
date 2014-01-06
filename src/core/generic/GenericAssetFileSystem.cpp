#include <Xli/NativeFileSystem.h>

#ifdef WIN32
# include <Xli/Win32Header.h>
# include <Xli/Unicode.h>
# include <Xli/Path.h>
#endif

namespace Xli
{
    FileSystem* CreateAssetFileSystem()
    {
        return Disk->CreateSubFileSystem("data");
    }
}
