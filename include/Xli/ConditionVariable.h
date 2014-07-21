#ifndef __XLI_CONDITION_VARIABLE_H__
#define __XLI_CONDITION_VARIABLE_H__

#include <Xli/Mutex.h>
#include <Xli/Exception.h>

namespace Xli
{
    struct __CondImpl;

    /**
        \addtogroup XliThreading
        @{
    */

    typedef __CondImpl* CondHandle;

    CondHandle CreateCond();
    void DeleteCond(CondHandle handle);

    void CondWait(CondHandle handle, MutexHandle mutex);
    void CondSignal(CondHandle handle);
    void CondBroadcast(CondHandle handle);

    /** @} */

    /**
        \ingroup XliThreading
    */
    class ConditionVariable
    {
        CondHandle handle;

        ConditionVariable(const ConditionVariable& copy);
        ConditionVariable& operator = (const ConditionVariable& copy);

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
