#ifndef __XLI_FILESYSTEM_H__
#define __XLI_FILESYSTEM_H__

#include <Xli/File.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    enum FileFlag
    {
        FileFlagReadOnly = 1 << 0,
        FileFlagDirectory = 1 << 1
    };

    /**
        \ingroup XliIO
    */
    struct FileInfo
    {
        String Name;
        UInt64 Size;
        UInt32 Flags;
        Timestamp CreationTime;
        Timestamp LastAccessTime;
        Timestamp LastWriteTime;
    };

    /**
        \ingroup XliIO
    */
    class FileSystem: public Object
    {
    public:
        virtual ~FileSystem();

        virtual Stream* OpenFile(const String& filename, FileMode mode = FileModeRead) = 0;
        virtual DataAccessor* OpenFileAsBuffer(const String& filename);

        virtual void CreateDirectory(const String& name);
        virtual void DeleteDirectory(const String& name);
        virtual void DeleteFile(const String& name);

        virtual void MoveDirectory(const String& oldName, const String& newName);
        virtual void MoveFile(const String& oldName, const String& newName);

        virtual bool GetFileInfo(const String& path, FileInfo& result);
        virtual void GetFiles(const String& path, Array<FileInfo>& list);

        virtual void GetFiles(Array<FileInfo>& list);

        virtual bool Exists(const String& path);
        virtual bool IsFile(const String& path);
        virtual bool IsDirectory(const String& path);

        /**
            Makes sure all directories in the path exists.
        */
        virtual void CreateDirectories(const String& path);

        /**
            Removes directory and all contents.
        */
        virtual void DeleteDirectoryRecursive(const String& name);

        virtual FileSystem* CreateSubFileSystem(const String& path);
    };
}

#endif
