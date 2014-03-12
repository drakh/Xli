#ifndef __XLI_HASH_H__
#define __XLI_HASH_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
    /**
        \addtogroup Xli
        @{
    */

    UInt32 Hash(const UInt8* data, int size);

    UInt32 Hash(bool b);
    UInt32 Hash(const void* p);
    UInt32 Hash(float f);
    UInt32 Hash(double d);

    UInt32 Hash(Int8 i);
    UInt32 Hash(UInt8 i);
    UInt32 Hash(Int16 i);
    UInt32 Hash(UInt16 i);
    UInt32 Hash(Int32 i);
    UInt32 Hash(UInt32 i);
    UInt32 Hash(Int64 i);
    UInt32 Hash(UInt64 i);

    template <typename T> 
    UInt32 Hash(const T* t) 
    { 
        return Hash((const void*)t); 
    }
    
    template <typename T> 
    UInt32 Hash(T* t) 
    { 
        return Hash((const void*)t); 
    }

    template <typename T> 
    UInt32 Hash(const T& t) 
    { 
        return Hash((const UInt8*)&t, (int)sizeof(T)); 
    }

    /** @} */

    /**
        \ingroup XliCoreUtils
    */
    template <typename T> struct HashTraits
    {
        static UInt32 Hash(const T& key) 
        {
            return Xli::Hash(key); 
        }
        
        static bool Equals(const T& a, const T& b) 
        {
            return a == b; 
        }
    };
}

#endif
