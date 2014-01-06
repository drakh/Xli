#include <Xli/ToString.h>
#include <cstdio>

namespace Xli
{
    String ToString(void* p)
    {
        char buf[64];
        sprintf_s(buf, 64, "0x%llx", (unsigned long long int)p);
        return buf;
    }

    String ToString(int i)
    {
        return i;
    }

    String ToString(unsigned int i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%u", i);
        return buf;
    }

    String ToString(short i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%hd", i);
        return buf;
    }

    String ToString(unsigned short i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%hu", i);
        return buf;
    }

    String ToString(long i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%ld", i);
        return buf;
    }

    String ToString(unsigned long i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%lu", i);
        return buf;
    }

    String ToString(long long i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%lld", i);
        return buf;
    }

    String ToString(unsigned long long i)
    {
        char buf[64];
        sprintf_s(buf, 64, "%llu", i);
        return buf;
    }

    String ToString(float f)
    {
        return f;
    }

    String ToString(double d)
    {
        return d;
    }

    String ToString(char c)
    {
        return c;
    }

    String ToString(unsigned char c)
    {
        return (int)c;
    }

    String ToString(bool b)
    {
        return String::FromBool(b);
    }
}
