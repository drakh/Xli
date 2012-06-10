#ifndef __XLI_GRAPHICS_ARRAY_BUFFER_H__
#define __XLI_GRAPHICS_ARRAY_BUFFER_H__

#include <Xli/Managed.h>
#include <Xli/Buffer.h>

namespace Xli
{
	class ArrayBuffer: public DataAccessor
	{
		uint stride;
		Managed<DataAccessor> dataAccessor;

	public:
		ArrayBuffer(uint stride, uint elmCount);
		ArrayBuffer(DataAccessor* data, uint stride);

		uint GetStride() const;
		virtual unsigned int GetSizeInBytes() const;

		UInt8* GetData();
		virtual const UInt8* GetData() const;

		UInt8* GetElementPtr(uint index);
		const UInt8* GetElementPtr(uint index) const;

		DataAccessor* GetDataAccessor();
	};
}

#endif
