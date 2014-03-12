#ifndef __XLI_HASH_H__
#define __XLI_HASH_H__

#include <Xli/BaseTypes.h>
#include <Xli/String.h>

namespace Xli
{
    /**
        \addtogroup XliCoreTraits
        @{
    */

    template <typename T>
    bool Equals(const T& a, const T& b)
    {
        return a == b;
    }

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
    UInt32 Hash(const String& str);

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

    String ToString(void* p);
    String ToString(int i);
    String ToString(unsigned int i);
    String ToString(short i);
    String ToString(unsigned short i);
    String ToString(long i);
    String ToString(unsigned long i);
    String ToString(long long i);
    String ToString(unsigned long long i);
    String ToString(float f);
    String ToString(double d);
    String ToString(char c);
    String ToString(unsigned char c);
    String ToString(bool b);

    template <typename T> 
    String ToString(const T& t)
    {
        return t.ToString();
    }

    /** @} */

    /**
        \ingroup XliCoreTraits
    */
    template <typename T> struct Traits
    {
        static bool Equals(const T& a, const T& b) 
        {
            return Xli::Equals(a, b);
        }
        
        static UInt32 Hash(const T& t) 
        {
            return Xli::Hash(t);
        }

        static String ToString(const T& t)
        {
            return Xli::ToString(t);
        }
    };
}

#endif
