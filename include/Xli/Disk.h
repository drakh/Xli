#ifndef __XLI_DISK_H__
#define __XLI_DISK_H__

#include <Xli/FileSystem.h>

namespace Xli
{
    /**
        \ingroup XliCoreIO
    */
    enum SystemDirectory
    {
        SystemDirectoryConfig, //< Returns the path on the file system where to store roaming app data
        SystemDirectoryData, //< Returns the path on the file system where to store local app data
        SystemDirectoryDesktop,
        SystemDirectoryDownloads,
        SystemDirectoryTemplates,
        SystemDirectoryPublic,
        SystemDirectoryDocuments, //< Returns the path on the file system where the user keeps his personal documents. E.g. the "My Documents" folder
        SystemDirectoryMusic,
        SystemDirectoryPictures,
        SystemDirectoryVideos,
    };

    /**
        \ingroup XliCoreIO
    */
    class NativeFileSystem : public FileSystem
    {
    public:
        /**
            Generates and returns a unique filename that can be used for creating a temporary file. The temporary file must be deleted manually by the creator.
        */
        virtual String CreateTempFilename() = 0;

        /**
            Returns a system directory
        */
        virtual String GetSystemDirectory(SystemDirectory dir) = 0;

        /**
            Returns the directory where application is run from
        */
        virtual String GetBaseDirectory() = 0;

        /**
            Returns the working directory
        */
        virtual String GetCurrentDirectory() = 0;

        /**
            Sets the working directory
        */
        virtual void ChangeDirectory(const String& dir) = 0;

        static void Init();
        static void Done();
    };

    /**
        \ingroup XliCoreIO
    */
    class DiskAccessor
    {
    public:
        NativeFileSystem* operator ->();
        operator FileSystem*();
    };

    /**
        \ingroup XliCoreIO
    */
    extern DiskAccessor Disk;
}


#endif
