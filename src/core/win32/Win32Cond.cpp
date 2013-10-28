#include <Xli/ConditionVariable.h>
#include <Xli/PlatformSpecific/Win32Header.h>

namespace Xli
{
	CondHandle CreateCond()
	{
		CONDITION_VARIABLE* cv = new CONDITION_VARIABLE;
		InitializeConditionVariable(cv);
		return cv;
	}

	void DeleteCond(CondHandle handle)
	{
		delete (CONDITION_VARIABLE*)handle;
	}

	void CondWait(CondHandle handle, MutexHandle mutex)
	{
		SleepConditionVariableCS((CONDITION_VARIABLE*)handle, (CRITICAL_SECTION*)mutex, INFINITE);
	}

	void CondSignal(CondHandle handle)
	{
		WakeConditionVariable((CONDITION_VARIABLE*)handle);
	}

	void CondBroadcast(CondHandle handle)
	{
		WakeAllConditionVariable((CONDITION_VARIABLE*)handle);
	}
}
