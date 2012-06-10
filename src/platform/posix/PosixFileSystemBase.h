#include <XliPlatform/NativeFileSystem.h>
#include <Xli/File.h>
#include <XliPlatform/Time.h>
#include <Xli/Random.h>
#include <Xli/Hash.h>
#include <Xli/DateTime.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>

namespace Xli
{
	class PosixFileSystemBase: public NativeFileSystem
	{
	public:
		static Timestamp ConvertToTimeStamp(time_t time)
		{
			return (Timestamp)time * DateTime::PerSecond + (369 * 365 + 89) * DateTime::PerDay;
		}

		virtual Stream* OpenFile(const String& fileName, FileMode mode)
		{
			return new File(fileName, mode);
		}

        virtual String GetTempDirectory()
        {
			char* tmpdir = getenv("TMPDIR");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TEMP");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TEMPDIR");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = getenv("TMP");
			if (tmpdir == 0 || strlen(tmpdir) == 0) tmpdir = (char*)"/tmp";
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
					tmp[i] = (StringChar)cs[rand.NextInt(cl)];
				}

				tmp = pre + tmp;

				if (!FileExists(tmp))
				{
					return tmp;
				}
			}
		}

		virtual String GetDocumentsDirectory()
		{
			char* home = getenv("HOME");
			if (home == 0 || strlen(home) == 0) home = (char*)"~";
			return (String)home;
		}

		virtual String GetRoamingAppDataDirectory()
		{
			return GetDocumentsDirectory() + "/.config";
		}

		virtual String GetLocalAppDataDirectory()
		{
			return GetDocumentsDirectory() + "/.local/share";
		}

		virtual String GetCurrentDirectory()
		{
			char buf[1024];
			char* ptr = getcwd(buf, 1024);
			if (ptr != buf) XLI_THROW("Unable to get current directory");
			return String(buf);
		}
		
		virtual void ChangeDirectory(const String& dir)
		{
			if (!dir.Length()) return;
			CharString path = dir;

			if (chdir(path.Data()) != 0)
			{
				XLI_THROW("Unable to change directory to '" + path + "'");
			}
		}

		virtual void CreateDirectory(const String& name)
		{
			if (!name.Length()) return;
			if (name == "~") return;

			CharString path = name;

			struct stat st;
			if (stat(path.Data(), &st) == 0) return;

			if (mkdir(path.Data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
			{
				if (errno == EEXIST) return;
				XLI_THROW("Unable to create directory '" + path + "'");
			}
		}

		virtual void DeleteDirectory(const String& name)
		{ 
			CharString path = name;

			if (!rmdir(path.Data()) != 0)
			{
				XLI_THROW("Unable to delete directory '" + path + "'");
			}
		}

		virtual void DeleteFile(const String& name)
		{ 
			CharString path = name;

			if (!unlink(path.Data()) != 0)
			{
				XLI_THROW("Unable to delete file '" + path + "'");
			}
		}

		virtual void MoveDirectory(const String& oldName, const String& newName)
		{ 
			CharString oldPath = oldName, newPath = newName;

			if (rename(oldPath.Data(), newPath.Data()) != 0)
			{
				XLI_THROW("Unable to move directory '" + oldPath + "' to '" + newPath + "'");
			}
		}
		
		virtual void MoveFile(const String& oldName, const String& newName)
		{ 
			CharString oldPath = oldName, newPath = newName;

			if (rename(oldPath.Data(), newPath.Data()) != 0)
			{
				XLI_THROW("Unable to move file '" + oldPath + "' to '" + newPath + "'");
			}
		}

		virtual bool FileExists(const String& path)
		{
			CharString ascii_path = path;

			struct stat attributes;
			return stat(ascii_path.Data(), &attributes) != -1;
		}

		virtual FileInfo GetFileInfo(const String& path)
		{
			CharString ascii_path = path;

			struct stat attributes;

			if (stat(ascii_path.Data(), &attributes) == -1)
			{
				XLI_THROW_FILE_NOT_FOUND(path);
			}

			FileInfo f;
			f.Name = ascii_path;

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

		virtual void GetFiles(const String& path, Array<FileInfo>& list)
		{
			CharString ascii_path = path;

			if (ascii_path.Length() > 0 && ascii_path.Last() != '/')
			{
				ascii_path = ascii_path + "/";
			}

			DIR *dp;
			struct dirent *ep;

			if ((dp  = opendir(ascii_path.Data())) == NULL)
			{
				XLI_THROW_FILE_NOT_FOUND(path);
			}

            if (ascii_path == "./")
			{
				ascii_path = "";
			}

			while ((ep = readdir(dp)) != NULL)
			{
				String fn = ep->d_name;
				if (fn == "." || fn == "..") continue;
				list.Add(GetFileInfo(ascii_path + fn));
			}

			closedir(dp);
		}
	};
}
