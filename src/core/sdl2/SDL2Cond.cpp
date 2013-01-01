#include <XliPlatform/ConditionVariable.h>
#include <SDL.h>

namespace Xli
{
	CondHandle CreateCond()
	{
		return (CondHandle)SDL_CreateCond();
	}

	void DeleteCond(CondHandle handle)
	{
		SDL_DestroyCond((SDL_cond*)handle);
	}

	void CondWait(CondHandle handle, MutexHandle mutex)
	{
		SDL_CondWait((SDL_cond*)handle, (SDL_mutex*)mutex);
	}

	void CondSignal(CondHandle handle)
	{
		SDL_CondSignal((SDL_cond*)handle);
	}

	void CondBroadcast(CondHandle handle)
	{
		SDL_CondBroadcast((SDL_cond*)handle);
	}
}
