#ifndef __XLI_MUTEX_H__
#define __XLI_MUTEX_H__

#include <Xli/Object.h>
#include <Xli/Exception.h>

namespace Xli
{
    struct __MutexImpl;

    /**
        \addtogroup XliCoreThreading
        @{
    */
    typedef __MutexImpl* MutexHandle;

    MutexHandle CreateMutex();
    void DeleteMutex(MutexHandle mutex);

    void LockMutex(MutexHandle mutex);
    void UnlockMutex(MutexHandle mutex);

    /** @} */

    /**
        \ingroup XliCoreThreading
    */
    class Mutex
    {
        MutexHandle handle;

        Mutex(const Mutex& copy);
        Mutex& operator = (const Mutex& copy);

    public:
        Mutex()
        {
            handle = CreateMutex();
        }

        ~Mutex()
        {
            DeleteMutex(handle);
        }

        void Lock()
        {
            LockMutex(handle);
        }

        void Unlock()
        {
            UnlockMutex(handle);
        }

        operator MutexHandle()
        {
            return handle;
        }
    };

    /**
        \ingroup XliCoreThreading
    */
    class MutexLock
    {
        MutexHandle handle;
    
        MutexLock(const MutexLock& copy);
        MutexLock& operator = (const MutexLock& copy);

    public:
        MutexLock(MutexHandle handle)
        {
            LockMutex(this->handle = handle);
        }

        ~MutexLock()
        {
            UnlockMutex(this->handle);
        }
    };
}

#endif
