#include "PosixFileSystemBase.h"

namespace Xli
{
	NativeFileSystem* CreateNativeFileSystem()
	{
		return new PosixFileSystemBase();
	}
}
