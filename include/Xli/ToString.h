#ifndef __XLI_TO_STRING_H__
#define __XLI_TO_STRING_H__

#include <Xli/String.h>

namespace Xli
{
    /**
        \addtogroup Xli
        @{
    */

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
}


#endif
