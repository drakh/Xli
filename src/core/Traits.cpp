#include <Xli/Traits.h>
#include <cstdio>

namespace Xli
{
    UInt32 DefaultTraits::Hash(const UInt8* data, int size)
    {
        // djb2
        UInt32 hash = 5381;

        for (int i = 0; i < size; i++)
            hash = ((hash << 5) + hash) ^ (UInt32)data[i];

        return hash;
    }

    UInt32 DefaultTraits::Hash(bool b)
    {
        return b ? 1 : 0; 
    }
    
    UInt32 DefaultTraits::Hash(const void* p)
    { 
        return DefaultTraits::Hash((UInt8*)&p, sizeof(void*));
    }
    
    UInt32 DefaultTraits::Hash(float f)
    { 
        return *(UInt32*)&f; 
    }
    
    UInt32 DefaultTraits::Hash(double d)
    { 
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&d)[0];
        hash = (13 * hash) + ((UInt32*)&d)[1];
        return hash;
    }

    UInt32 DefaultTraits::Hash(Int8 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(UInt8 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(Int16 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(UInt16 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(Int32 i)
    {
        return (UInt32)i; 
    }
    
    UInt32 DefaultTraits::Hash(UInt32 i)
    {
        return i; 
    }
    
    UInt32 DefaultTraits::Hash(Int64 i)
    {
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&i)[0];
        hash = (13 * hash) + ((UInt32*)&i)[1];
        return hash;
    }
    
    UInt32 DefaultTraits::Hash(UInt64 i)
    {
        UInt32 hash = 27;
        hash = (13 * hash) + ((UInt32*)&i)[0];
        hash = (13 * hash) + ((UInt32*)&i)[1];
        return hash;
    }

    UInt32 DefaultTraits::Hash(const String& str)
    {
        return Hash((const UInt8*)str.DataPtr(), str.Length());
    }

    String DefaultTraits::ToString(void* p)
    {
        char buf[64];
        sprintf_s(buf, 64, "0x%llx", (unsigned long long int)p);
        return buf;
    }

    String DefaultTraits::ToString(int i)
    {
        return i;
    }

    String DefaultTraits::ToString(unsigned int i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%u", i);
        return buf;
    }

    String DefaultTraits::ToString(short i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%hd", i);
        return buf;
    }

    String DefaultTraits::ToString(unsigned short i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%hu", i);
        return buf;
    }

    String DefaultTraits::ToString(long i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%ld", i);
        return buf;
    }

    String DefaultTraits::ToString(unsigned long i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%lu", i);
        return buf;
    }

    String DefaultTraits::ToString(long long i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%lld", i);
        return buf;
    }

    String DefaultTraits::ToString(unsigned long long i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%llu", i);
        return buf;
    }

    String DefaultTraits::ToString(float f)
    {
        return f;
    }

    String DefaultTraits::ToString(double d)
    {
        return d;
    }

    String DefaultTraits::ToString(char c)
    {
        return c;
    }

    String DefaultTraits::ToString(unsigned char c)
    {
        return (int)c;
    }

    String DefaultTraits::ToString(bool b)
    {
        return String::FromBool(b);
    }
}
