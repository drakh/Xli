#ifndef __XLI_PLATFORM_SDL2_FILE_SYSTEM_H__
#define __XLI_PLATFORM_SDL2_FILE_SYSTEM_H__

#include "../posix/PosixFileSystemBase.h"

namespace Xli
{
	class SDL2FileSystem : public PosixFileSystemBase
	{
	public:
		virtual Stream* OpenFile(const String& filename, FileMode mode);
        
#ifdef XLI_PLATFORM_IOS
        
        virtual String GetTempDirectory();
        virtual String GetSystemDirectory(SystemDirectory dir);
        
#endif
	};
}


#endif
