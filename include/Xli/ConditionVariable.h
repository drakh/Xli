#ifndef __XLI_CONDITION_VARIABLE_H__
#define __XLI_CONDITION_VARIABLE_H__

#include "Mutex.h"

namespace Xli
{
    /**
        \addtogroup XliCoreThreading
        @{
    */

    typedef void* CondHandle;

    CondHandle CreateCond();
    void DeleteCond(CondHandle handle);

    void CondWait(CondHandle handle, MutexHandle mutex);
    void CondSignal(CondHandle handle);
    void CondBroadcast(CondHandle handle);

    /** @} */

    /**
        \ingroup XliCoreThreading
    */
    class ConditionVariable
    {
        CondHandle handle;

    public:
        ConditionVariable()
        {
            handle = CreateCond();
        }

        ~ConditionVariable()
        {
            DeleteCond(handle);
        }

        void Wait(MutexHandle mutex)
        {
            CondWait(handle, mutex);
        }

        void Signal()
        {
            CondSignal(handle);
        }

        void Broadcast()
        {
            CondBroadcast(handle);
        }
    };
}


#endif
