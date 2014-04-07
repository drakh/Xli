#ifndef __XLI_BUNDLE_H__
#define __XLI_BUNDLE_H__

#include <Xli/FileSystem.h>

namespace Xli
{
    /**
        \ingroup XliCoreIO
    */
    class BundleFileSystem
    {
    public:
        static void Init();
        static void Done();
    };

    /**
        \ingroup XliCoreIO
    */
    class BundleAccessor
    {
    public:
        void SetFilesystem(FileSystem* fs);
        FileSystem* operator ->();
        operator FileSystem*();
    };

    /**
        \ingroup XliCoreIO
    */
    extern BundleAccessor Bundle;
}


#endif
