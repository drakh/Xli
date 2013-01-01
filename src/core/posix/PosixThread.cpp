#include <XliPlatform.h>

#include <pthread.h>
#include <time.h>

namespace Xli
{
	ThreadHandle CreateThread(void (entrypoint(void*)), void* arg)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		pthread_t thread;
		int rc = pthread_create(&thread, &attr, (void *(*)(void *))entrypoint, arg);

		if (rc)
		{
			XLI_THROW("Error creating pthread: " + (String)rc);
		}

		pthread_attr_destroy(&attr);
		return (ThreadHandle)thread;
	}

	void WaitForThread(ThreadHandle handle)
	{
		pthread_t thread = (pthread_t)handle;
		int rc = pthread_join(thread, NULL);

		if (rc)
		{
			XLI_THROW("Error joining pthread: " + (String)rc);
		}
	}

	void KillThread(ThreadHandle handle)
	{
#ifdef ANDROID
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
#else
		pthread_cancel((pthread_t)handle);
#endif
	}

	void Sleep(int ms)
	{
		struct timespec t, r;
		t.tv_sec = ms / 1000;
		t.tv_nsec = (ms % 1000) * 1000 * 1000;

		int rc = nanosleep(&t, &r);

		if (rc)
		{
			XLI_THROW("nanosleep failed: " + (String)rc);
		}
	}
}
