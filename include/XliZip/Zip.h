#ifndef __XLI_ZIP_H__
#define __XLI_ZIP_H__

#include <Xli/Object.h>
#include <Xli/FileSystem.h>
#include <Xli/File.h>

namespace Xli
{
    /**
        \ingroup XliZip
    */
    class Zip
    {
        static FileSystem* Open(Stream* file, const char* password);

    public:
        static XLI_INLINE FileSystem* Open(Stream* file) 
        { 
            return Open(file, 0); 
        }
        
        static XLI_INLINE FileSystem* Open(Stream* file, const String& password) 
        { 
            return Open(file, password.DataPtr()); 
        }
        
        static XLI_INLINE FileSystem* Open(const String& filename) 
        { 
            Managed<File> f = new File(filename, FileModeRead); 
            return Open(f); 
        }
        
        static XLI_INLINE FileSystem* Open(const String& filename, const String& password) 
        { 
            Managed<File> f = new File(filename, FileModeRead); 
            return Open(f, password.DataPtr()); 
        }
    };
}

#endif
