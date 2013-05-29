#include <Xli/Thread.h>
#include <Xli/Console.h>
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
		if (pthread_create(&thread, &attr, (void *(*)(void *))entrypoint, arg))	
			XLI_THROW("Failed to create thread");

		pthread_attr_destroy(&attr);
		return (ThreadHandle)thread;
	}

	void WaitForThread(ThreadHandle handle)
	{
		pthread_t thread = (pthread_t)handle;
		int rc = pthread_join(thread, NULL);
		if (rc) ErrorPrintLine("XLI ERROR: pthread_join failed: " + String::HexFromInt(rc));
	}

	void Sleep(int ms)
	{
		struct timespec t, r;
		t.tv_sec = ms / 1000;
		t.tv_nsec = (ms % 1000) * 1000 * 1000;

		int rc = nanosleep(&t, &r);
		if (rc)	ErrorPrintLine("XLI ERROR: nanosleep failed: " + String::HexFromInt(rc));
	}
}
