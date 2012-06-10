#ifndef __XLI_DATA_TYPE_H__
#define __XLI_DATA_TYPE_H__

#include "BaseTypes.h"

namespace Xli
{
	enum DataType
	{
		DataTypeHalf = 1,
		DataTypeFloat = 2,
		DataTypeDouble = 3,
		DataTypeUInt64 = 4,
		DataTypeUInt32 = 5,
		DataTypeUInt24 = 6,
		DataTypeUInt16 = 7,
		DataTypeUInt8 = 8,
		DataTypeInt64 = 9,
		DataTypeInt32 = 10,
		DataTypeInt24 = 11,
		DataTypeInt16 = 12,
		DataTypeInt8 = 13
	};

	class DataTypeInfo
	{
	public:
		static unsigned int SizeOf(DataType dt);
		static bool IsSigned(DataType dt);
		static const char* ToString(DataType dt);
	};

	class Endian
	{
	public:
		template <typename T> static T Swap4(const T& i)
		{
			T r;
			UInt8 *src = (UInt8*)&i, *dst = (UInt8*)&r;
			dst[0] = src[3];
			dst[1] = src[2];
			dst[2] = src[1];
			dst[3] = src[0];
			return r;
		}
		template <typename T> static T Swap2(const T& i)
		{
			T r;
			UInt8 *src = (UInt8*)&i, *dst = (UInt8*)&r;
			dst[0] = src[1];
			dst[1] = src[0];
			return r;
		}
	};
}


#endif
