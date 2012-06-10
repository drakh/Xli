#include <XliPlatform/Mutex.h>
#include <SDL.h>

namespace Xli
{
	MutexHandle CreateMutex()
	{
		return (MutexHandle)SDL_CreateMutex();
	}

	void DeleteMutex(MutexHandle mutex)
	{
		SDL_DestroyMutex((SDL_mutex*)mutex);
	}

	void LockMutex(MutexHandle mutex)
	{
		SDL_mutexP((SDL_mutex*)mutex);
	}

	void UnlockMutex(MutexHandle mutex)
	{
		SDL_mutexV((SDL_mutex*)mutex);
	}
}
