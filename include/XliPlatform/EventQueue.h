#ifndef __XLI_PLATFORM_EVENT_QUEUE_H__
#define __XLI_PLATFORM_EVENT_QUEUE_H__

#include <XliPlatform/Mutex.h>
#include <XliPlatform/ConditionVariable.h>
#include <Xli/Queue.h>

namespace Xli
{
	template <typename T> class EventQueue
	{
		Mutex mutex;
		ConditionVariable cond;
		Queue<T> events;

	public:
		void Post(const T& e)
		{
			mutex.Lock();
			events.Enqueue(e);
			cond.Signal();
			mutex.Unlock();
		}

		T Get()
		{
			mutex.Lock();

			if (events.IsEmpty())
			{
				cond.Wait(mutex);
			}

			T e = events.Dequeue();
			mutex.Unlock();
			return e;
		}

		bool Poll(T& e)
		{
			mutex.Lock();
			bool b = !events.IsEmpty();
			if (b) e = events.Dequeue();
			mutex.Unlock();
			return b;
		}

		void Flush()
		{
			mutex.Lock();
			events.Clear();
			mutex.Unlock();
		}
	};
}

#endif
