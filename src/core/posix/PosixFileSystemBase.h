#include <Xli/Disk.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        class PosixFileSystemBase: public NativeFileSystem
        {
        public:
            virtual Stream* OpenFile(const String& filename, FileMode mode);

            virtual String GetTempDirectory() = 0;

            virtual String CreateTempFilename();

            virtual String GetCurrentDirectory();
            virtual void ChangeDirectory(const String& path);

            virtual void CreateDirectory(const String& path);

            virtual void DeleteDirectory(const String& path);
            virtual void DeleteFile(const String& path);

            virtual void MoveDirectory(const String& oldPath, const String& newPath);
            virtual void MoveFile(const String& oldPath, const String& newPath);

            virtual bool GetFileInfo(const String& path, FileInfo& result);
            virtual void GetFiles(const String& _path, Array<FileInfo>& list);
        };
    }
}
