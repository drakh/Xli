#ifndef __XLI_THREAD_H__
#define __XLI_THREAD_H__

#include <Xli/Window.h>
#include <Xli/Shared.h>

namespace Xli
{
    struct __ThreadImpl;

    /**
        \addtogroup XliCoreThreading
        @{
    */
    typedef __ThreadImpl* ThreadHandle;

    ThreadHandle CreateThread(void (*entrypoint)(void*), void* arg);
    void WaitForThread(ThreadHandle thread);

    void* GetCurrentThread();

    void Sleep(int ms);

    /** @} */

    /**
        \ingroup XliCoreThreading
    */
    class Task: public Object
    {
        friend class Thread;
        volatile bool stopped;

    public:
        Task();
        virtual ~Task() {}

        bool IsStopped();

        virtual void Run() = 0;

        virtual String ToString() const;
    };

    /**
        \ingroup XliCoreThreading
    */
    class Thread
    {
        ThreadHandle handle;
        Shared<Task> task;

        static void thread_func(void* arg);

        Thread(const Thread& copy);
        Thread& operator = (const Thread& copy);

    public:
        /**
            Creates a new Thread.
            @param task Task to be executed by thread. If specified, the thread will start immediately without having to call Start() explicitly.
        */
        Thread(Managed<Task> task = 0);
        ~Thread();

        void Start(Managed<Task> task);
        void Wait();

        bool HasStarted();
        bool IsDone();
    };
}

#endif
