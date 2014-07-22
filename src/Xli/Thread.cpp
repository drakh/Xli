#include <Xli/Thread.h>
#include <Xli/TextWriter.h>
#include <Xli/Console.h>

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
            // TODO: Needs exception handler interface
            ErrorPrintLine("Thread: " + task->ToString());
            ErrorPrintLine(e.ToString());

            //MessageBox::HandleException(e, "Thread: " + task->ToString());
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