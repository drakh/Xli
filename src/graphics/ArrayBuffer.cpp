#include <XliGraphics/ArrayBuffer.h>

namespace Xli
{
	ArrayBuffer::ArrayBuffer(uint stride, uint elmCount)
	{
		this->stride = stride;
		this->dataAccessor = Buffer::Create(stride * elmCount);
	}
	ArrayBuffer::ArrayBuffer(DataAccessor* data, uint stride)
	{
		this->stride = stride;
		this->dataAccessor = Assert(data);
		this->dataAccessor->AddRef();
	}
	uint ArrayBuffer::GetStride() const
	{
		return stride;
	}
	UInt8* ArrayBuffer::GetData()
	{
		return const_cast<UInt8*>(dataAccessor->GetData());
	}
	const UInt8* ArrayBuffer::GetData() const
	{
		return dataAccessor->GetData();
	}
	UInt8* ArrayBuffer::GetElementPtr(uint index)
	{
		return (UInt8*)dataAccessor->GetData() + stride*index;
	}
	const UInt8* ArrayBuffer::GetElementPtr(uint index) const
	{
		return (UInt8*)dataAccessor->GetData() + stride*index;
	}
	unsigned int ArrayBuffer::GetSizeInBytes() const
	{
		return dataAccessor->GetSizeInBytes();
	}
	DataAccessor* ArrayBuffer::GetDataAccessor()
	{
		return dataAccessor;
	}
}
