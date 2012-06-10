#ifndef __XLI_THREAD_H__
#define __XLI_THREAD_H__

#include "Window.h"

namespace Xli
{
	typedef void* ThreadHandle;

	ThreadHandle CreateThread(void (entrypoint(void*)), void* arg);
	void WaitForThread(ThreadHandle h);
	void KillThread(ThreadHandle h);
	void Sleep(int ms);

	class ThreadJob: public Object
	{
		friend class Thread;
		volatile bool stopped;

	public:
		ThreadJob();
		virtual ~ThreadJob() {}
		bool IsStopped();

		virtual void Run() = 0;

		virtual String ToString();
	};

	class Thread
	{
		ThreadHandle handle;
		Shared<ThreadJob> job;

		static void thread_func(void* arg);

	public:
		/**
			Creates a new Thread.
			@param job Job to be executed by thread. If specified, the thread will start immediately without having to call Start() explicitly.
		*/
		Thread(ThreadJob* job = 0);
		~Thread();

		void Start(ThreadJob* job);
		void Wait();
		void Kill();

		bool HasStarted();
		bool IsDone();
	};
}

#endif
