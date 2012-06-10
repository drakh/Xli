#include <XliPlatform.h>
#include <windows.h>

namespace Xli
{
	ThreadHandle CreateThread(void (entrypoint(void*)), void* arg)
	{
		return (ThreadHandle)::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)entrypoint, arg, 0, 0);
	}

	void WaitForThread(ThreadHandle handle)
	{
		DWORD res = ::WaitForSingleObject((HANDLE)handle, INFINITE);
		if (res != WAIT_OBJECT_0)
		{
			XLI_THROW("Unable to wait for thread");
		}
		::CloseHandle((HANDLE)handle);
	}

	void KillThread(ThreadHandle handle)
	{
		::TerminateThread((HANDLE)handle, -1);
		::CloseHandle((HANDLE)handle);
	}

	void Sleep(int ms)
	{
		::Sleep(ms);
	}
}
