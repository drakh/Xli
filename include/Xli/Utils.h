#ifndef __XLI_UTILS_H__
#define __XLI_UTILS_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
    /**
        \addtogroup XliCoreUtils
        @{
    */
    template <typename T> 
    void Swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

    /** @} */

    /**
        \ingroup XliCoreUtils
    */
    template <typename T> struct ComparatorLessThan
    {
        static bool Compare(const T& a, const T& b)
        {
            return a < b;
        }
    };

    /**
        \ingroup XliCoreUtils
    */
    template <typename T> struct ComparatorGreaterThan
    {
        static bool Compare(const T& a, const T& b)
        {
            return a > b;
        }
    };

    /**
        \ingroup XliCoreUtils
    */
    template <typename T> struct ComparatorPointerLessThan
    {
        static bool Compare(const T& a, const T& b)
        {
            return *a < *b;
        }
    };

    /**
        \ingroup XliCoreUtils
    */
    template <typename T> struct ComparatorPointerGreaterThan
    {
        static bool Compare(const T& a, const T& b)
        {
            return *a > *b;
        }
    };
}

#endif

