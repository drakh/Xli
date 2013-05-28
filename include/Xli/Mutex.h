#ifndef __XLI_MUTEX_H__
#define __XLI_MUTEX_H__

#include <Xli/Object.h>

namespace Xli
{
	/**
		\addtogroup Threading
		@{
	*/
	typedef void* MutexHandle;

	MutexHandle CreateMutex();
	void DeleteMutex(MutexHandle mutex);

	void LockMutex(MutexHandle mutex);
	void UnlockMutex(MutexHandle mutex);

	/** @} */

	/**
		\ingroup Threading
	*/
	class Mutex
	{
		MutexHandle handle;

	public:
		Mutex()
		{
			handle = CreateMutex();
		}

		~Mutex()
		{
			DeleteMutex(handle);
		}

		inline void Lock()
		{
			LockMutex(handle);
		}

		inline void Unlock()
		{
			UnlockMutex(handle);
		}

		operator MutexHandle()
		{
			return handle;
		}
	};
}

#endif
