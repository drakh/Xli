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
#ifdef WIN32

		WCHAR buf[MAX_PATH];
		DWORD result = GetModuleFileNameW(GetModuleHandle(0), buf, MAX_PATH);

		if (result > 0 && result < MAX_PATH)
		{
			String exe = Unicode::Utf16To8(buf, (int)result);
			String dir = Path::GetDirectoryName(exe);

			if (Disk->IsDirectory(dir))
				return Disk->CreateSubFileSystem(dir + "/data");
		}

#endif

		return Disk->CreateSubFileSystem(Disk->GetCurrentDirectory() + "/data");
	}
}
