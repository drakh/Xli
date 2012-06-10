#include <Xli/Buffer.h>

namespace Xli
{
	Buffer::Buffer(unsigned int size)
	{
		this->size = size;
		this->data = ((UInt8*)this) + sizeof(Buffer);
	}
	
	void* Buffer::operator new (size_t size, unsigned int bufSize)
	{
		return (Buffer*)malloc(sizeof(Buffer) + bufSize);
	}
	void Buffer::operator delete (void* buf, unsigned int bufSize)
	{
		free(buf);
	}
	void Buffer::operator delete (void* buf)
	{
		free(buf);
	}
	
	Buffer* Buffer::Create(unsigned int size)
	{
		return new (size) Buffer(size);
	}

	void Buffer::Delete()
	{
		this->~Buffer();
		this->operator delete(this, this->size);
	}

	Buffer* Buffer::Copy(const void* data, unsigned int size)
	{
		Buffer* b = Buffer::Create(size);
		memcpy(b->Data(), data, size);
		return b;
	}
}
