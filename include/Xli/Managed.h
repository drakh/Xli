#ifndef __XLI_MANAGED_H__
#define __XLI_MANAGED_H__

#include <Xli/Config.h>
#include <Xli/Exception.h>

namespace Xli
{
    /**
        \ingroup XliCoreContainers
    */
    template <class T> class Managed
    {
    protected:
        T* object;

    public:
        Managed()
        {
            this->object = 0;
        }

        Managed(T* object)
        {
            this->object = object;
        }

        Managed(const Managed& managed)
        {
            this->object = managed.object;
            if (object) object->AddRef();
        }

        ~Managed()
        {
            if (object) object->Release();
        }

        bool IsNull() const
        {
            return !object;
        }

        bool IsSet() const
        {
            return object != 0;
        }

        Managed& operator = (T* ptr)
        {
            if (object) object->Release();
            object = ptr;
            return *this;
        }

        Managed& operator = (const Managed& managed)
        {
            if (object) object->Release();
            object = managed.object;
            if (object) object->AddRef();
            return *this;
        }

        T* Get()
        {
#ifdef XLI_NULL_POINTER_CHECK
            if (IsNull()) XLI_THROW_NULL_POINTER;
#endif
            return object;
        }

        const T* Get() const
        {
#ifdef XLI_NULL_POINTER_CHECK
            if (IsNull()) XLI_THROW_NULL_POINTER;
#endif
            return object;
        }

        T* Pointer()
        {
            return object;
        }

        const T* Pointer() const
        {
            return object;
        }

        operator T*()
        {
            return object;
        }

        operator const T*() const
        {
            return object;
        }

        T* operator ->()
        {
            return Get();
        }

        const T* operator ->() const
        {
            return Get();
        }
    };

    /**
        \ingroup XliCoreContainers
    */
    template <class T> Managed<T> ManagePtr(T* object)
    {
        return object;
    }
}


#endif
