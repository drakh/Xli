#ifndef __XLI_BUFFER_H__
#define __XLI_BUFFER_H__

#include <Xli/BaseTypes.h>
#include <Xli/Object.h>
#include <stdlib.h>
#include <new>

namespace Xli
{
	class DataAccessor: public Object
	{
	public:
		virtual const UInt8* GetData() const = 0;
		virtual unsigned int GetSizeInBytes() const = 0;
	};

	/**
		A buffer of bytes.
		A buffer can not be resized or relocated, so other objects can depend on the pointer to the buffer data.
		Buffer objects are meant to be constructed using an overloaded new operator which takes bufSize as an argument.
		@see Buffer::operator new
	*/
	class Buffer: public DataAccessor
	{
		unsigned int size;
		UInt8* data;
		
		Buffer(unsigned int size);
		void* operator new (size_t size, unsigned int bufSize);
		void operator delete (void* buf, unsigned int bufSize);

	public:
		static Buffer* Create(unsigned int size);
		static Buffer* Copy(const void* data, unsigned int size);

		void operator delete (void* buf);

		virtual void Delete();

		/**
			Returns the size of the buffer, in bytes.
			@return The size of the buffer, in bytes.
		*/
		unsigned int Size() const
		{
			return size;
		}

		/**
			Allows the buffer to be used as pointer.
			This pointer will never change or become invalid during the lifetime of the buffer.
			@returns A pointer to the data in the buffer.
		*/
		UInt8* Data()
		{
			return data;
		}
		const UInt8* Data() const
		{
			return data;
		}

		virtual const UInt8* GetData() const
		{
			return data;
		}
		virtual unsigned int GetSizeInBytes() const
		{
			return size;
		}
	};

	template <unsigned int BufSize> class StaticBuffer: public DataAccessor
	{
		UInt8 data[BufSize];

	public:
		unsigned int Size() const
		{
			return BufSize;
		}

		UInt8* Data()
		{
			return data;
		}
		const UInt8* Data() const
		{
			return data;
		}

		virtual const UInt8* GetData() const
		{
			return data;
		}
		virtual unsigned int GetSizeInBytes() const
		{
			return BufSize;
		}
	};

	class BufferPointer: public DataAccessor
	{
		UInt8* data;
		unsigned int size;
		bool ownsData;

	public:
		BufferPointer(void* data, unsigned int size, bool ownsData): data((UInt8*)data), size(size), ownsData(ownsData) {}
		~BufferPointer() { if (ownsData) delete [] (UInt8*)data; }

		unsigned int Size() const
		{
			return size;
		}

		UInt8* Data()
		{
			return data;
		}
		const UInt8* Data() const
		{
			return data;
		}

		virtual const UInt8* GetData() const
		{
			return data;
		}
		virtual unsigned int GetSizeInBytes() const
		{
			return size;
		}
	};

	class BufferReference: public DataAccessor
	{
		UInt8* data;
		unsigned int size;
		Object* owner;

	public:
		BufferReference(void* data, unsigned int size, Object* owner)
			: data((UInt8*)data), size(size), owner(owner)
		{
			if (owner != 0) owner->AddRef();
		}
		virtual ~BufferReference()
		{
			if (owner != 0) owner->Release();
		}

		virtual const UInt8* GetData() const
		{
			return data;
		}
		virtual unsigned int GetSizeInBytes() const
		{
			return size;
		}
	};
}

#endif
