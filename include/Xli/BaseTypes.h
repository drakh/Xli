#ifndef __XLI_BASE_TYPES_H__
#define __XLI_BASE_TYPES_H__

#include <Xli/Config.h>

namespace Xli
{
    /**
        \addtogroup Xli
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

#ifdef WIN32
    typedef wchar_t Utf16;
#else
    typedef UInt16 Utf16;
#endif

    typedef UInt32 Utf32;
    typedef UInt64 Timestamp;

    /** @} */

    /**
        \addtogroup XliCoreContainers
        @{
    */

    template <typename T, int TBufSize = 4> class Array;

    /** @} */

    // Verify size of base types at compile time
    typedef char __TestInt8[sizeof(Int8) == 1 ? 1 : -1];
    typedef char __TestInt16[sizeof(Int16) == 2 ? 1 : -1];
    typedef char __TestInt32[sizeof(Int32) == 4 ? 1 : -1];
    typedef char __TestInt64[sizeof(Int64) == 8 ? 1 : -1];
    typedef char __TestUInt8[sizeof(UInt8) == 1 ? 1 : -1];
    typedef char __TestUInt16[sizeof(UInt16) == 2 ? 1 : -1];
    typedef char __TestUInt32[sizeof(UInt32) == 4 ? 1 : -1];
    typedef char __TestUInt64[sizeof(UInt64) == 8 ? 1 : -1];
}

#endif
