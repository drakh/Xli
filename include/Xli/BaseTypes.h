#ifndef __XLI_BASE_TYPES_H__
#define __XLI_BASE_TYPES_H__

#include <Xli/Config.h>

namespace Xli
{
	typedef signed char Int8;
	typedef signed short Int16;
	typedef signed int Int32;
	typedef signed long long Int64;
	
	typedef unsigned char UInt8;
	typedef unsigned short UInt16;
	typedef unsigned int UInt32;
	typedef unsigned long long UInt64;

	// Verify size of base types at compile time
	typedef char __TestInt16[sizeof(Int16) == 2 ? 1 : -1];
	typedef char __TestInt32[sizeof(Int32) == 4 ? 1 : -1];
	
	
	typedef UInt16 Utf16Char;
	typedef UInt32 Utf32Char;

	typedef UInt64 Timestamp;
}

#ifndef XLI_HAS_UINT
typedef unsigned int uint;
#endif

namespace Xli
{
	template <typename T, uint TBufSize = 4> class Array;
	template <typename T, uint TBufSize = 8> class Stringt;
}

#endif
