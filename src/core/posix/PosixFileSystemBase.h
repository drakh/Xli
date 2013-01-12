#include <Xli/NativeFileSystem.h>
#include <Xli/File.h>
#include <Xli/Time.h>
#include <Xli/Random.h>
#include <Xli/Hash.h>
#include <Xli/DateTime.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>

namespace Xli
{
	class PosixFileSystemBase: public NativeFileSystem
	{
	public:
		static Timestamp ConvertToTimeStamp(time_t time)
		{
			return (Timestamp)time * DateTime::PerSecond + (369 * 365 + 89) * DateTime::PerDay;
		}

		virtual Stream* OpenFile(const String& filename, FileMode mode)
		{
			return new File(filename, mode);
		}

        virtual String GetTempDirectory()
        {
			const char* tmpdir = getenv("TMPDIR");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TEMP");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TEMPDIR");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TMP");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = "/tmp";
            return tmpdir;
        }
        
		virtual String CreateTempFilename()
		{
			const String pre = GetTempDirectory() + "/" + (int)getpid() + "-";

			Random rand(Hash(GetTimestamp()));
			static const char* cs = "-_1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
			int cl = strlen(cs);

			while (true)
			{
				String tmp = String::Create(8);

				for (int i = 0; i < tmp.Length(); i++)
				{
					tmp[i] = cs[rand.NextInt(cl)];
				}

				tmp = pre + tmp;

				if (!FileExists(tmp))
				{
					return tmp;
				}
			}
		}

		virtual String GetSystemDirectory(SystemDirectory dir)
		{
			const char* home = getenv("HOME");
			if (home == 0 || strlen(home) == 0) home = "~";

			switch (dir)
			{
			case SystemDirectoryDocuments:
				return home;

			case SystemDirectoryRoamingAppData:
				return (String)home + "/.config";

			case SystemDirectoryLocalAppData:
				return (String)home + "/.local/share";

			default:
				return ".";
			}
		}

		virtual String GetCurrentDirectory()
		{
			char buf[1024];
			const char* ptr = getcwd(buf, 1024);
			if (ptr != buf) XLI_THROW("Unable to get current directory");
			return buf;
		}
		
		virtual void ChangeDirectory(const String& path)
		{
			if (!path.Length()) return;

			if (chdir(path.Data()) != 0)
			{
				XLI_THROW("Unable to change directory to '" + path + "'");
			}
		}

		virtual void CreateDirectory(const String& path)
		{
			if (!path.Length()) return;
			if (path == "~") return;

			struct stat st;
			if (stat(path.Data(), &st) == 0) return;

			if (mkdir(path.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
			{
				if (errno == EEXIST) return;
				XLI_THROW("Unable to create directory '" + path + "'");
			}
		}

		virtual void DeleteDirectory(const String& path)
		{ 
			if (!rmdir(path.Data()) != 0)
			{
				XLI_THROW("Unable to delete directory '" + path + "'");
			}
		}

		virtual void DeleteFile(const String& path)
		{ 
			if (!unlink(path.Data()) != 0)
			{
				XLI_THROW("Unable to delete file '" + path + "'");
			}
		}

		virtual void MoveDirectory(const String& oldPath, const String& newPath)
		{ 
			if (rename(oldPath.Data(), newPath.Data()) != 0)
			{
				XLI_THROW("Unable to move directory '" + oldPath + "' to '" + newPath + "'");
			}
		}
		
		virtual void MoveFile(const String& oldPath, const String& newPath)
		{ 
			if (rename(oldPath.Data(), newPath.Data()) != 0)
			{
				XLI_THROW("Unable to move file '" + oldPath + "' to '" + newPath + "'");
			}
		}

		virtual bool FileExists(const String& path)
		{
			struct stat attributes;
			return stat(path.Data(), &attributes) != -1;
		}

		virtual FileInfo GetFileInfo(const String& path)
		{
			struct stat attributes;

			if (stat(path.Data(), &attributes) == -1)
			{
				XLI_THROW_FILE_NOT_FOUND(path);
			}

			FileInfo f;
			f.Name = path;

			f.Flags = 0;
			if (!(((attributes.st_mode & S_IWOTH) == S_IWOTH)
				|| (attributes.st_gid == getgid() && ((attributes.st_mode & S_IWGRP) == S_IWGRP))
				|| (attributes.st_uid == getuid() && ((attributes.st_mode & S_IWUSR) == S_IWUSR))))
			{
				f.Flags |= FileFlagReadOnly;
			}

			if (S_ISDIR(attributes.st_mode))
			{
				f.Flags |= FileFlagDirectory;
			}

			f.CreationTime = ConvertToTimeStamp(attributes.st_mtime);
			f.LastAccessTime = ConvertToTimeStamp(attributes.st_atime);
			f.LastWriteTime = ConvertToTimeStamp(attributes.st_mtime);

			return f;
		}

		virtual void GetFiles(const String& _path, Array<FileInfo>& list)
		{
			String path;
			if (_path.Length() > 0 && _path.Last() != '/')
			{
				path = _path + "/";
			}
			else
			{
				path = _path;
			}

			DIR *dp;
			struct dirent *ep;

			if ((dp  = opendir(path.Data())) == NULL)
			{
				XLI_THROW_FILE_NOT_FOUND(path);
			}

            if (path == "./")
			{
				path = "";
			}

			while ((ep = readdir(dp)) != NULL)
			{
				String fn = ep->d_name;
				if (fn == "." || fn == "..") continue;
				list.Add(GetFileInfo(path + fn));
			}

			closedir(dp);
		}
	};
}
