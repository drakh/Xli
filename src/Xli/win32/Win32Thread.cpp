#include <Xli/Thread.h>
#include <Xli/PlatformSpecific/Win32Header.h>
#include <process.h>

namespace Xli
{
    struct Win32ThreadData
    {
        void (*Entrypoint)(void*);
        void* Arg;
    };

    unsigned int __stdcall Win32ThreadFunc(void* data)
    {
        Win32ThreadData* win32Data = reinterpret_cast<Win32ThreadData*>(data);
        win32Data->Entrypoint(win32Data->Arg);
        delete win32Data;
        return 0;
    }

    ThreadHandle CreateThread(void (*entrypoint)(void*), void* arg)
    {
        //HANDLE thread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entrypoint, arg, 0, 0);

        Win32ThreadData* data = new Win32ThreadData;
        data->Entrypoint = entrypoint;
        data->Arg = arg;

        unsigned threadId;
        HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, &Win32ThreadFunc, data, 0, &threadId);
        
        if (!thread) 
            XLI_THROW("Failed to create thread");
        
        return (ThreadHandle)thread;
    }

    void WaitForThread(ThreadHandle handle)
    {
        DWORD res = ::WaitForSingleObject((HANDLE)handle, INFINITE);

        if (res != WAIT_OBJECT_0)
            ErrorPrintLine("XLI ERROR: WaitForSingleObject failed");
        else
            ::CloseHandle((HANDLE)handle);
    }

    void* GetCurrentThread()
    {
        return (void*)::GetCurrentThread();
    }

    void Sleep(int ms)
    {
        ::Sleep(ms);
    }
}
