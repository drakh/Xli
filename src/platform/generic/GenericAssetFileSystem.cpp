#include <XliPlatform/NativeFileSystem.h>

namespace Xli
{
	FileSystem* CreateAssetFileSystem()
	{
		return Disk->CreateSubFileSystem("data");
	}
}
