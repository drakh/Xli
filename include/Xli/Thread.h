#ifndef __XLI_THREAD_H__
#define __XLI_THREAD_H__

#include "Window.h"

namespace Xli
{
	/**
		\addtogroup XliCoreThreading
		@{
	*/
	typedef void* ThreadHandle;

	ThreadHandle CreateThread(void (entrypoint(void*)), void* arg);
	void WaitForThread(ThreadHandle h);
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

	public:
		/**
			Creates a new Thread.
			@param task Task to be executed by thread. If specified, the thread will start immediately without having to call Start() explicitly.
		*/
		Thread(Task* task = 0);
		~Thread();

		void Start(Task* task);
		void Wait();

		bool HasStarted();
		bool IsDone();
	};
}

#endif
