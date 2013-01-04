#ifndef __XLI_DATA_TYPE_H__
#define __XLI_DATA_TYPE_H__

#include "BaseTypes.h"

namespace Xli
{
	/**
		\ingroup Core
	*/
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

	/**
		\ingroup Core
	*/
	class DataTypeInfo
	{
	public:
		static int SizeOf(DataType dt);
		static bool IsSigned(DataType dt);
		static const char* ToString(DataType dt);
	};
}


#endif
