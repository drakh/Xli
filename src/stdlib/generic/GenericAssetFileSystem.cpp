#include <Xli/NativeFileSystem.h>

namespace Xli
{
	FileSystem* CreateAssetFileSystem()
	{
		return Disk->CreateSubFileSystem("data");
	}
}
