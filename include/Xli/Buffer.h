#ifndef __XLI_BUFFER_H__
#define __XLI_BUFFER_H__

#include <Xli/IntTypes.h>
#include <Xli/Object.h>
#include <cstddef>
#include <new>

namespace Xli
{
    /**
        \ingroup XliBuffer
    */
    class DataAccessor: public Object
    {
    public:
        virtual const UInt8* GetDataPtr() const = 0;
        virtual int GetSizeInBytes() const = 0;
    };

    /**
        \ingroup XliBuffer

        A buffer of bytes.
        A buffer can not be resized or relocated, so other objects can depend on the pointer to the buffer data.
    */
    class Buffer: public DataAccessor
    {
        int size;
        UInt8* data;
        
        Buffer(int size);
        void* operator new (size_t size, int bufSize);
        void operator delete (void* buf, int bufSize);
        void operator delete (void* buf);

    public:
        static Buffer* Create(int size);
        static Buffer* CopyFrom(const void* data, int size);

        virtual void Delete();

        /**
            Returns the size of the buffer, in bytes.
            @return The size of the buffer, in bytes.
        */
        int Size() const;

        /**
            Allows the buffer to be used as pointer.
            This pointer will never change or become invalid during the lifetime of the buffer.
            @returns A pointer to the data in the buffer.
        */
        UInt8* DataPtr();
        const UInt8* DataPtr() const;

        virtual const UInt8* GetDataPtr() const;
        virtual int GetSizeInBytes() const;
    };

    /**
        \ingroup XliBuffer
    */
    class BufferPtr: public DataAccessor
    {
        UInt8* data;
        int size;
        bool ownsData;

    public:
        BufferPtr(void* data, int size, bool ownsData);
        virtual ~BufferPtr();

        int Size() const;

        UInt8* DataPtr();
        const UInt8* DataPtr() const;

        virtual const UInt8* GetDataPtr() const;
        virtual int GetSizeInBytes() const;
    };

    /**
        \ingroup XliBuffer
    */
    class BufferRef: public DataAccessor
    {
        UInt8* data;
        int size;
        Object* owner;

    public:
        BufferRef(void* data, int size, Object* owner);
        virtual ~BufferRef();

        virtual const UInt8* GetDataPtr() const;
        virtual int GetSizeInBytes() const;
    };
}

#endif
