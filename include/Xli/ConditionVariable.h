#ifndef __XLI_CONDITION_VARIABLE_H__
#define __XLI_CONDITION_VARIABLE_H__

#include "Mutex.h"

namespace Xli
{
	/**
		\addtogroup Threading
		@{
	*/

	typedef void* CondHandle;

	CondHandle CreateCond();
	void DeleteCond(CondHandle handle);

	void CondWait(CondHandle handle, MutexHandle mutex);
	void CondSignal(CondHandle handle);
	void CondBroadcast(CondHandle handle);

	/** @} */

	/**
		\ingroup Threading
	*/
	class ConditionVariable
	{
		CondHandle handle;

	public:
		ConditionVariable()
		{
			handle = CreateCond();
		}

		~ConditionVariable()
		{
			DeleteCond(handle);
		}

		inline void Wait(MutexHandle mutex)
		{
			CondWait(handle, mutex);
		}

		inline void Signal()
		{
			CondSignal(handle);
		}

		inline void Broadcast()
		{
			CondBroadcast(handle);
		}
	};
}


#endif
