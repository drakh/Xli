//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <Xli/Thread.h>
#include <Xli/Console.h>
#include <XliPlatform/PlatformSpecific/Win32Header.h>
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
            Err->WriteLine("XLI ERROR: WaitForSingleObject failed");
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
