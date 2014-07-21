#include <Xli/Buffer.h>
#include <cstdlib>
#include <cstring>

namespace Xli
{
    Buffer::Buffer(int size)
    {
        this->size = size;
        this->data = ((UInt8*)this) + sizeof(Buffer);
    }
    
    void* Buffer::operator new (size_t size, int bufSize)
    {
        return malloc(sizeof(Buffer) + bufSize);
    }

    void Buffer::operator delete (void* buf, int bufSize)
    {
        free(buf);
    }

    void Buffer::operator delete (void* buf)
    {
        free(buf);
    }
    
    Buffer* Buffer::Create(int size)
    {
        return new (size) Buffer(size);
    }

    void Buffer::Delete()
    {
        this->~Buffer();
        free(this);
    }

    Buffer* Buffer::CopyFrom(const void* data, int size)
    {
        Buffer* b = Buffer::Create(size);
        memcpy(b->DataPtr(), data, size);
        return b;
    }

    int Buffer::Size() const
    {
        return size;
    }

    UInt8* Buffer::DataPtr()
    {
        return data;
    }

    const UInt8* Buffer::DataPtr() const
    {
        return data;
    }

    const UInt8* Buffer::GetDataPtr() const
    {
        return data;
    }

    int Buffer::GetSizeInBytes() const
    {
        return size;
    }

    BufferPtr::BufferPtr(void* data, int size, bool ownsData)
        : data((UInt8*)data), size(size), ownsData(ownsData) 
    {
    }

    BufferPtr::~BufferPtr() 
    { 
        if (ownsData) delete [] (UInt8*)data; 
    }

    int BufferPtr::Size() const
    {
        return size;
    }

    UInt8* BufferPtr::DataPtr()
    {
        return data;
    }

    const UInt8* BufferPtr::DataPtr() const
    {
        return data;
    }

    const UInt8* BufferPtr::GetDataPtr() const
    {
        return data;
    }

    int BufferPtr::GetSizeInBytes() const
    {
        return size;
    }

    BufferRef::BufferRef(void* data, int size, Object* owner)
        : data((UInt8*)data), size(size), owner(owner)
    {
        if (owner != 0) owner->AddRef();
    }

    BufferRef::~BufferRef()
    {
        if (owner != 0) owner->Release();
    }

    const UInt8* BufferRef::GetDataPtr() const
    {
        return data;
    }

    int BufferRef::GetSizeInBytes() const
    {
        return size;
    }
}
