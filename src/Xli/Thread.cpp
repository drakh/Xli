//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <Xli/Thread.h>
#include <Xli/TextWriter.h>
#include <Xli/CoreLib.h>

namespace Xli
{
    void Thread::thread_func(void* arg)
    {
        Thread* thread = (Thread*)arg;
        Task* task = thread->task;
        
        task->stopped = false;

        try
        {
            task->Run();
        }
        catch (const Exception& e)
        {
            CoreLib::OnUnhandledException(e, task->ToString());
        }

        task->stopped = true;
    }

    Thread::Thread(Managed<Task> task)
    {
        this->handle = 0;

        if (!task.IsNull())
            Start(task);
    }

    Thread::~Thread()
    {
        if (handle)
            Wait();
    }

    bool Thread::IsDone()
    {
        return !handle || !task || task->stopped;
    }

    bool Thread::HasStarted()
    {
        return handle != 0;
    }

    void Thread::Start(Managed<Task> task)
    {
        if (task.IsNull())
            XLI_THROW_NULL_POINTER;

        if (handle)
            XLI_THROW("Thread is already started");

        this->task = task;
        this->handle = CreateThread(thread_func, (void*)this);
    }

    void Thread::Wait()
    {
        if (!handle)
            return;

        task->stopped = true;
        WaitForThread(handle);
        handle = 0;
        task = 0;
    }

    Task::Task()
    {
        stopped = true;
    }

    bool Task::IsStopped()
    {
        return stopped;
    }

    String Task::ToString() const
    {
        return "<Task " + DefaultTraits::ToString((void*)this) + ">";
    }
}
