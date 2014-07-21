#ifndef __XLI_SHARED_H__
#define __XLI_SHARED_H__

#include <Xli/Managed.h>

namespace Xli
{
    /**
    \ingroup XliContainers
    */
    template <typename T> class Shared : public Managed<T>
    {
    public:
        Shared()
        {
        }

        Shared(T* ptr)
            : Managed<T>(ptr)
        {
            if (this->object)
                this->object->AddRef();
        }

        Shared(const Shared& shared)
            : Managed<T>(shared.object)
        {
            if (this->object)
                this->object->AddRef();
        }

        Shared(const Managed<T>& managed) 
            : Managed<T>(managed)
        {
        }

        ~Shared()
        {
        }

        Shared& operator = (T* ptr)
        {
            if (ptr) 
                ptr->AddRef();
            
            if (this->object) 
                this->object->Release();
            
            this->object = ptr;
            return *this;
        }

        Shared& operator = (const Managed<T>& managed)
        {
            T* ptr = const_cast<T*>(managed.Pointer());
            
            if (ptr) 
                ptr->AddRef();
            
            if (this->object) 
                this->object->Release();
            
            this->object = ptr;
            return *this;
        }

        Shared& operator = (const Shared& shared)
        {
            if (shared.object) 
                shared.object->AddRef();
            
            if (this->object) 
                this->object->Release();
            
            this->object = shared.object;
            return *this;
        }

        operator T*()
        {
            return this->object;
        }

        operator const T*() const
        {
            return this->object;
        }
    };

    /**
        \ingroup XliContainers
    */
    template <typename T> 
    T* SharePtr(T* object)
    {
        if (object) 
            object->AddRef();

        return object;
    }
}


#endif
