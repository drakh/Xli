#include <XliPlatform/Mutex.h>
#include <pthread.h>

namespace Xli
{
	MutexHandle CreateMutex()
	{
		pthread_mutex_t* handle = new pthread_mutex_t;
		pthread_mutex_init(handle, NULL);
		return (MutexHandle)handle;
	}

	void DeleteMutex(MutexHandle handle)
	{
		pthread_mutex_destroy((pthread_mutex_t*)handle);
		delete (pthread_mutex_t*)handle;
	}

	void LockMutex(MutexHandle handle)
	{
		pthread_mutex_lock((pthread_mutex_t*)handle);
	}

	void UnlockMutex(MutexHandle handle)
	{
		pthread_mutex_unlock((pthread_mutex_t*)handle);
	}
}
