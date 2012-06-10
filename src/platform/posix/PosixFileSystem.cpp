#include "PosixFileSystemBase.h"

namespace Xli
{
	class PosixFileSystem: public PosixFileSystemBase
	{
	};

	NativeFileSystem* CreateNativeFileSystem()
	{
		return new PosixFileSystem();
	}
}
