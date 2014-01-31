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

        pthread_t result;
        if (pthread_create(&result, &attr, (void *(*)(void *))entrypoint, arg))    
            XLI_THROW("Failed to create thread");

        pthread_attr_destroy(&attr);
        return (ThreadHandle)result;
    }

    void WaitForThread(ThreadHandle handle)
    {
        pthread_t thread = (pthread_t)handle;
        int result = pthread_join(thread, NULL);
        
        if (result) 
            ErrorPrintLine("XLI ERROR: pthread_join failed: " + String::HexFromInt(result));
    }

    void* GetCurrentThread()
    {
        return (void*)pthread_self();
    }

    void Sleep(int ms)
    {
        struct timespec t, r;
        t.tv_sec = ms / 1000;
        t.tv_nsec = (ms % 1000) * 1000 * 1000;
        
        int result = nanosleep(&t, &r);
        
        if (result)
            ErrorPrintLine("XLI ERROR: nanosleep failed: " + String::HexFromInt(result));
    }
}
