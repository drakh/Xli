#ifndef __XLI_BASE_TYPES_H__
#define __XLI_BASE_TYPES_H__

#include <Xli/Config.h>

namespace Xli
{
	/**
		\addtogroup Core
		@{
	*/

	typedef signed char Int8;
	typedef signed short Int16;
	typedef signed int Int32;
	typedef signed long long Int64;
	
	typedef unsigned char UInt8;
	typedef unsigned short UInt16;
	typedef unsigned int UInt32;
	typedef unsigned long long UInt64;

	// Verify size of base types at compile time
	typedef char __TestInt8[sizeof(Int8) == 1 ? 1 : -1];
	typedef char __TestInt16[sizeof(Int16) == 2 ? 1 : -1];
	typedef char __TestInt32[sizeof(Int32) == 4 ? 1 : -1];
	typedef char __TestInt64[sizeof(Int64) == 8 ? 1 : -1];
	
	typedef UInt64 Timestamp;

	template <typename T, int TBufSize = 4> class Array;

	/** @} */
}

#endif
