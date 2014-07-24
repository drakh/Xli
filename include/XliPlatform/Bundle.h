#ifndef __XLI_BUNDLE_H__
#define __XLI_BUNDLE_H__

#include <Xli/FileSystem.h>

namespace Xli
{
    /**
        \ingroup XliPlatform
    */
    class BundleFileSystem
    {
    public:
        static void Init();
        static void Done();
    };

    /**
        \ingroup XliPlatform
    */
    class BundleAccessor
    {
    public:
        void SetFilesystem(FileSystem* fs);
        FileSystem* operator ->();
        operator FileSystem*();
    };

    /**
        \ingroup XliPlatform
    */
    extern BundleAccessor Bundle;
}


#endif
