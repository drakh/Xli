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
        refCount++;
    }

    void Object::Release()
    {
        refCount--;

        if (!refCount) 
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
