#include <XliPlatform/Mutex.h>
#include <XliPlatform/Win32Windows.h>

namespace Xli
{
	MutexHandle CreateMutex()
	{
		CRITICAL_SECTION* cs = new CRITICAL_SECTION;
		InitializeCriticalSection(cs);
		return cs;
	}

	void DeleteMutex(MutexHandle mutex)
	{
		DeleteCriticalSection((CRITICAL_SECTION*)mutex);
		delete (CRITICAL_SECTION*)mutex;
	}

	void LockMutex(MutexHandle mutex)
	{
		EnterCriticalSection((CRITICAL_SECTION*)mutex);
	}

	void UnlockMutex(MutexHandle mutex)
	{
		LeaveCriticalSection((CRITICAL_SECTION*)mutex);
	}
}
