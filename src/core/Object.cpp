#include <Xli/Atomic.h>
#include <Xli/Object.h>
#include <Xli/Exception.h>
#include <Xli/ToString.h>

namespace Xli
{
    Object::Object() 
    { 
        refCount = 1; 
    }

    Object::~Object() 
    {
        if (refCount > 1)
            XLI_THROW_BAD_DELETE;
    }

    int Object::GetRefCount() const
    {
        return refCount;
    }

    void Object::AddRef() 
    {
        AtomicIncrement(&refCount);
    }

    void Object::Release()
    {
        if (AtomicDecrement(&refCount) == 0)
            Delete();
    }

    void Object::Delete() 
    { 
        delete this; 
    }

    String Object::ToString() const
    {
        return "<Object " + Xli::ToString((void*)this) + ">";
    }
}
