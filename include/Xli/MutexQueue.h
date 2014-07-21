#ifndef __XLI_MUTEX_QUEUE_H__
#define __XLI_MUTEX_QUEUE_H__

#include <Xli/Mutex.h>
#include <Xli/Queue.h>

namespace Xli
{
    /**
        \ingroup XliContainers
    */
    template <typename T> class MutexQueue
    {
        Mutex mutex;
        Queue<T> queue;

    public:
        void Trim()
        {
            MutexLock lock(mutex);
            queue.Trim();
        }

        void Clear()
        {
            MutexLock lock(mutex);
            queue.Clear();
        }

        void Enqueue(const T& value)
        {
            MutexLock lock(mutex);
            queue.Enqueue(value);
        }

        T Dequeue()
        {
            MutexLock lock(mutex);
            return queue.Dequeue();
        }

        int Count()
        {
            MutexLock lock(mutex);
            return queue.Count();
        }

        bool IsEmpty()
        {
            MutexLock lock(mutex);
            return queue.IsEmpty();
        }
    };
}


#endif
