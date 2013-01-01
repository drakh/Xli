#ifndef __XLI_THREAD_H__
#define __XLI_THREAD_H__

#include "Window.h"

namespace Xli
{
	/**
		\addtogroup Threading
		@{
	*/
	typedef void* ThreadHandle;

	ThreadHandle CreateThread(void (entrypoint(void*)), void* arg);
	void WaitForThread(ThreadHandle h);
	void KillThread(ThreadHandle h);
	void Sleep(int ms);

	/** @} */

	/**
		\ingroup Threading
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

		virtual String ToString();
	};

	/**
		\ingroup Threading
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
		void Kill();

		bool HasStarted();
		bool IsDone();
	};
}

#endif
