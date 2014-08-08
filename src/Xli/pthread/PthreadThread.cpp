#include <Xli/Thread.h>
#include <Xli/Console.h>
#include <pthread.h>
#include <time.h>
#include <cstdlib>

namespace Xli
{
    struct PthreadData
    {
        void (*Entrypoint)(void*);
        void* Arg;
    };

    void* PthreadFunc(void* data)
    {
        PthreadData* pdata = (PthreadData*)data;
        pdata->Entrypoint(pdata->Arg);
        free(data);
        return NULL;
    }

    ThreadHandle CreateThread(void (*entrypoint)(void*), void* arg)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        PthreadData* data = (PthreadData*)malloc(sizeof(PthreadData));
        data->Entrypoint = entrypoint;
        data->Arg = arg;

        pthread_t result;
        if (pthread_create(&result, &attr, PthreadFunc, data))    
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
