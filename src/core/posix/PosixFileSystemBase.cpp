#include "PosixFileSystemBase.h"

#include <Xli/Time.h>
#include <Xli/Random.h>
#include <Xli/Hash.h>
#include <Xli/DateTime.h>
#include <Xli/Path.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Xli
{
    namespace PlatformSpecific
    {
        static Timestamp ConvertToTimeStamp(time_t time)
        {
            return (Timestamp)time * DateTime::PerSecond + (369 * 365 + 89) * DateTime::PerDay;
        }

        Stream* PosixFileSystemBase::OpenFile(const String& filename, FileMode mode)
        {
            return new File(filename, mode);
        }

        String PosixFileSystemBase::CreateTempFilename()
        {
            String prefix = Path::Combine(GetTempDirectory(), (String)(int)getpid() + "-");

            Random rand(Hash(GetTimestamp()));
            static const char* cs = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            int cl = strlen(cs);

            while (true)
            {
                String tmp = String::Create(8);

                for (int i = 0; i < tmp.Length(); i++)
                    tmp[i] = cs[rand.NextInt(cl)];

                String result = prefix + tmp;

                struct stat attributes;
                if (stat(result.DataPtr(), &attributes) == -1)
                    return result;
            }
        }

        String PosixFileSystemBase::GetCurrentDirectory()
        {
            char buf[1024];
            const char* ptr = getcwd(buf, 1024);
            
            if (ptr != buf) 
                return ".";
            
            return buf;
        }
            
        void PosixFileSystemBase::ChangeDirectory(const String& path)
        {
            if (!path.Length()) 
                return;

            if (chdir(path.DataPtr()) != 0)
                XLI_THROW("Unable to change directory to '" + path + "'");
        }

        void PosixFileSystemBase::CreateDirectory(const String& path)
        {
            struct stat st;

            if (!path.Length() || 
                path == "~" ||
                stat(path.DataPtr(), &st) == 0) 
                return;

            if (mkdir(path.DataPtr(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
            {
                if (errno == EEXIST) 
                    return;

                XLI_THROW("Unable to create directory '" + path + "'");
            }
        }

        void PosixFileSystemBase::DeleteDirectory(const String& path)
        { 
            if (rmdir(path.DataPtr()) != 0)
                XLI_THROW("Unable to delete directory '" + path + "'");
        }

        void PosixFileSystemBase::DeleteFile(const String& path)
        { 
            if (unlink(path.DataPtr()) != 0)
                XLI_THROW("Unable to delete file '" + path + "'");
        }

        void PosixFileSystemBase::MoveDirectory(const String& oldPath, const String& newPath)
        { 
            if (rename(oldPath.DataPtr(), newPath.DataPtr()) != 0)
                XLI_THROW("Unable to move directory '" + oldPath + "' to '" + newPath + "'");
        }
            
        void PosixFileSystemBase::MoveFile(const String& oldPath, const String& newPath)
        { 
            if (rename(oldPath.DataPtr(), newPath.DataPtr()) != 0)
                XLI_THROW("Unable to move file '" + oldPath + "' to '" + newPath + "'");
        }

        bool PosixFileSystemBase::GetFileInfo(const String& path, FileInfo& f)
        {
            struct stat attributes;

            if (stat(path.DataPtr(), &attributes) == -1)
                return false;

            f.Name = path;

            f.Flags = 0;
            if (!(((attributes.st_mode & S_IWOTH) == S_IWOTH)
                || (attributes.st_gid == getgid() && ((attributes.st_mode & S_IWGRP) == S_IWGRP))
                || (attributes.st_uid == getuid() && ((attributes.st_mode & S_IWUSR) == S_IWUSR))))
                f.Flags |= FileFlagReadOnly;

            if (S_ISDIR(attributes.st_mode))
                f.Flags |= FileFlagDirectory;

            f.CreationTime = ConvertToTimeStamp(attributes.st_mtime);
            f.LastAccessTime = ConvertToTimeStamp(attributes.st_atime);
            f.LastWriteTime = ConvertToTimeStamp(attributes.st_mtime);

            return true;
        }

        void PosixFileSystemBase::GetFiles(const String& path, Array<FileInfo>& list)
        {
            String prefix = 
                path.Length() > 0 && path.Last() != '/' ? 
                    path + "/" : 
                    path;

            DIR *dp;
            struct dirent *ep;

            if ((dp = opendir(prefix.DataPtr())) == NULL)
                XLI_THROW_FILE_NOT_FOUND(prefix);

            if (prefix == "./")
                prefix = "";

            while ((ep = readdir(dp)) != NULL)
            {
                String fn = ep->d_name;
                
                if (fn == "." || fn == "..") 
                    continue;

                FileInfo info;
                if (GetFileInfo(prefix + fn, info))
                    list.Add(info);
            }

            closedir(dp);
        }
    }
}
