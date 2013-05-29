#include <Xli/Thread.h>
#include <Xli/Win32Header.h>

namespace Xli
{
	ThreadHandle CreateThread(void (entrypoint(void*)), void* arg)
	{
		ThreadHandle thread = (ThreadHandle)::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entrypoint, arg, 0, 0);
		if (!thread) XLI_THROW("Failed to create thread");
		return thread;
	}

	void WaitForThread(ThreadHandle handle)
	{
		DWORD res = ::WaitForSingleObject((HANDLE)handle, INFINITE);

		if (res != WAIT_OBJECT_0)
			ErrorPrintLine("XLI ERROR: WaitForSingleObject failed");
		else
			::CloseHandle((HANDLE)handle);
	}

	void Sleep(int ms)
	{
		::Sleep(ms);
	}
}
