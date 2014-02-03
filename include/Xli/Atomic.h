#ifndef __XLI_ATOMIC_H__
#define __XLI_ATOMIC_H__

#include <Xli/Config.h>

#ifdef XLI_COMPILER_MSVC
#include <intrin.h>
#endif

namespace Xli
{
    inline int AtomicIncrement(int* ptr)
    {
#ifdef XLI_COMPILER_MSVC
        return _InterlockedIncrement((volatile long*)&ptr);
#else
        return __sync_add_and_fetch(ptr, 1);
#endif
    }

    inline int AtomicDecrement(int* ptr)
    {
#ifdef XLI_COMPILER_MSVC
        return _InterlockedDecrement((volatile long*)&ptr);
#else
        return __sync_sub_and_fetch(ptr, 1);
#endif
    }
}

#endif
