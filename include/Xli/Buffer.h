#ifndef __XLI_BUFFER_H__
#define __XLI_BUFFER_H__

#include <Xli/BaseTypes.h>
#include <Xli/Object.h>
#include <stdlib.h>
#include <new>

namespace Xli
{
	/**
		\ingroup Core
	*/
	class DataAccessor: public Object
	{
	public:
		virtual const UInt8* GetData() const = 0;
		virtual int GetSizeInBytes() const = 0;
	};

	/**
		\ingroup Core

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
		static Buffer* Copy(const void* data, int size);

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
		UInt8* Data();
		const UInt8* Data() const;

		virtual const UInt8* GetData() const;
		virtual int GetSizeInBytes() const;
	};

	/**
		\ingroup Core
	*/
	class BufferPointer: public DataAccessor
	{
		UInt8* data;
		int size;
		bool ownsData;

	public:
		BufferPointer(void* data, int size, bool ownsData);
		virtual ~BufferPointer();

		int Size() const;

		UInt8* Data();
		const UInt8* Data() const;

		virtual const UInt8* GetData() const;
		virtual int GetSizeInBytes() const;
	};

	/**
		\ingroup Core
	*/
	class BufferReference: public DataAccessor
	{
		UInt8* data;
		int size;
		Object* owner;

	public:
		BufferReference(void* data, int size, Object* owner);
		virtual ~BufferReference();

		virtual const UInt8* GetData() const;
		virtual int GetSizeInBytes() const;
	};
}

#endif
