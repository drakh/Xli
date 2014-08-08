/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_THREAD_H__
#define __XLI_THREAD_H__

#include <Xli/Object.h>
#include <Xli/Shared.h>

namespace Xli
{
    struct __ThreadImpl;

    /**
        \addtogroup XliThreading
        @{
    */
    typedef __ThreadImpl* ThreadHandle;

    ThreadHandle CreateThread(void (*entrypoint)(void*), void* arg);
    void WaitForThread(ThreadHandle thread);

    void* GetCurrentThread();

    void Sleep(int ms);

    /** @} */

    /**
        \ingroup XliThreading
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
        \ingroup XliThreading
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
