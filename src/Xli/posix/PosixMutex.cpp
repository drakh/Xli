#include <Xli/Mutex.h>
#include <pthread.h>
#include <cstdlib>

namespace Xli
{
    MutexHandle CreateMutex()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        
        pthread_mutex_t* handle = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(handle, &attr);
        return (MutexHandle)handle;
    }

    void DeleteMutex(MutexHandle handle)
    {
        pthread_mutex_destroy((pthread_mutex_t*)handle);
        free(handle);
    }

    void LockMutex(MutexHandle handle)
    {
        pthread_mutex_lock((pthread_mutex_t*)handle);
    }

    void UnlockMutex(MutexHandle handle)
    {
        pthread_mutex_unlock((pthread_mutex_t*)handle);
    }
}
