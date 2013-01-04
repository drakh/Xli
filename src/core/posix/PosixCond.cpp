#include <Xli/ConditionVariable.h>
#include <pthread.h>

namespace Xli
{
	CondHandle CreateCond()
	{
		pthread_cond_t* handle = new pthread_cond_t;
		pthread_cond_init(handle, NULL);
		return (CondHandle)handle;
	}

	void DeleteCond(CondHandle handle)
	{
		pthread_cond_destroy((pthread_cond_t*)handle);
		delete (pthread_cond_t*)handle;
	}

	void CondWait(CondHandle handle, MutexHandle mutex)
	{
		pthread_cond_wait((pthread_cond_t*)handle, (pthread_mutex_t*)mutex);
	}

	void CondSignal(CondHandle handle)
	{
		pthread_cond_signal((pthread_cond_t*)handle);
	}

	void CondBroadcast(CondHandle handle)
	{
		pthread_cond_broadcast((pthread_cond_t*)handle);
	}
}
