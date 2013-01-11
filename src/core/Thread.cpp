#include <Xli/Thread.h>
#include <Xli/MessageBox.h>
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
			MessageBox::HandleException(e, "Thread: " + task->ToString());
		}

		task->stopped = true;
	}

	Thread::Thread(Task* task)
	{
		this->handle = 0;

		if (task)
		{
			Start(task);
		}
	}

	Thread::~Thread()
	{
		if (handle)
		{
			Wait();
		}
	}

	bool Thread::IsDone()
	{
		return handle == 0 || task.IsNull() || task->stopped;
	}

	bool Thread::HasStarted()
	{
		return handle != 0;
	}

	void Thread::Start(Task* task)
	{
		if (!task)
		{
			XLI_THROW_NULL_POINTER;
		}

		if (handle)
		{
			XLI_THROW("Thread is already started");
		}

		this->task = task;
		this->handle = CreateThread(thread_func, (void*)this);
	}

	void Thread::Wait()
	{
		if (!handle)
		{
			return;
		}

		task->stopped = true;
		WaitForThread(handle);
		handle = 0;
		task = 0;
	}

	void Thread::Kill()
	{
		if (!handle)
		{
			return;
		}

		task->stopped = true;
		KillThread(handle);
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

	String Task::ToString()
	{
		return "<Task " + Xli::ToString((void*)this) + ">";
	}
}
