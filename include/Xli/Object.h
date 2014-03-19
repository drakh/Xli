#ifndef __XLI_OBJECT_H__
#define __XLI_OBJECT_H__

#include <Xli/Config.h>

namespace Xli
{
    class String;

    class Object
    {
        int refCount;

        Object(const Object& copy);
        Object& operator = (const Object& copy);

    public:
        Object();
        virtual ~Object();

        int GetRefCount() const;

        void AddRef();
        void Release();

        virtual void Delete();
        virtual String ToString() const;
    };
}


#endif
