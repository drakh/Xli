#include "../posix/PosixFileSystemBase.h"
#include <Xli/Console.h>
#include <cstdlib>

namespace Xli
{
	class XdgFileSystem: public PosixFileSystemBase
	{
	public:
		String GetTempDirectory()
		{
			const char* tmpdir = getenv("TMPDIR");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TEMP");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TEMPDIR");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TMP");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = "/tmp";
			return tmpdir;
		}

		String GetSystemDirectory(SystemDirectory dir)
		{
			const char* homedir = getenv("HOME");
			if (homedir == 0 || strlen(homedir) == 0) homedir = "~";

			/*
				XDG_DESKTOP_DIR="$HOME/Desktop"
				XDG_DOWNLOAD_DIR="$HOME/Downloads"
				XDG_TEMPLATES_DIR="$HOME/Templates"
				XDG_PUBLICSHARE_DIR="$HOME/Public"
				XDG_DOCUMENTS_DIR="$HOME/Documents"
				XDG_MUSIC_DIR="$HOME/Music"
				XDG_PICTURES_DIR="$HOME/Pictures"
				XDG_VIDEOS_DIR="$HOME/Videos"
			*/

			switch (dir)
			{
			case SystemDirectoryDocuments:
				{
					const char* docsdir = getenv("XDG_DOCUMENTS_DIR");
					return docsdir != 0 && strlen(docsdir) ? docsdir : (String)homedir + "/Documents";
				}

			case SystemDirectoryRoamingAppData:
				{
					const char* configdir = getenv("XDG_CONFIG_HOME");
					return configdir != 0 && strlen(configdir) ? configdir : (String)homedir + "/.config";
				}

			case SystemDirectoryLocalAppData:
				{
					const char* datadir = getenv("XDG_DATA_HOME");
					return datadir != 0 && strlen(datadir) ? datadir : (String)homedir + "/.local/share";
				}

			default:
				ErrorPrintLine("XLI WARNING: Unknown system directory: " + (String)(int)dir);
				return homedir;
			}
		}
	};

	NativeFileSystem* CreateNativeFileSystem()
	{
		return new XdgFileSystem();
	}
}
