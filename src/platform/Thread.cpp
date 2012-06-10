#include <XliPlatform/Thread.h>
#include <XliPlatform/MessageBox.h>
#include <Xli/TextWriter.h>
#include <Xli/Console.h>

namespace Xli
{
	void Thread::thread_func(void* arg)
	{
		Thread* t = (Thread*)arg;
		ThreadJob* j = t->job;
		
		j->stopped = false;

		try
		{
			j->Run();
		}
		catch (const Exception& e)
		{
			MessageBox::HandleException(e, "Thread: " + j->ToString());
		}

		j->stopped = true;
	}

	Thread::Thread(ThreadJob* job)
	{
		this->handle = 0;

		if (job)
		{
			Start(job);
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
		return handle == 0 || job.IsNull() || job->stopped;
	}

	bool Thread::HasStarted()
	{
		return handle != 0;
	}

	void Thread::Start(ThreadJob* job)
	{
		if (!job)
		{
			XLI_THROW_NULL_POINTER;
		}

		if (handle)
		{
			XLI_THROW("Thread is already started");
		}

		this->job = job;
		this->handle = CreateThread(thread_func, (void*)this);
	}

	void Thread::Wait()
	{
		if (!handle)
		{
			return;
		}

		job->stopped = true;
		WaitForThread(handle);
		handle = 0;
		job = 0;
	}

	void Thread::Kill()
	{
		if (!handle)
		{
			return;
		}

		job->stopped = true;
		KillThread(handle);
		handle = 0;
		job = 0;
	}

	ThreadJob::ThreadJob()
	{
		stopped = true;
	}

	bool ThreadJob::IsStopped()
	{
		return stopped;
	}

	String ThreadJob::ToString()
	{
		return "<ThreadJob " + Xli::ToString((void*)this) + ">";
	}
}
