#include <Xli/Thread.h>
#include <SDL.h>

namespace Xli
{
	struct SDL_ThreadData
	{
		typedef void (*Func)(void*);
		SDL_Thread* Handle;
		Func Entrypoint;
		void* Argument;
	};

	int SDL_StartThread(void* arg)
	{
		SDL_ThreadData* data = (SDL_ThreadData*)arg;
		(data->Entrypoint)(data->Argument);
		return 0;
	}

	ThreadHandle CreateThread(void (entrypoint(void*)), void* arg)
	{
		SDL_ThreadData* data = new SDL_ThreadData;
		data->Entrypoint = entrypoint;
		data->Argument = arg;
		data->Handle = SDL_CreateThread(SDL_StartThread, "thread", data);
		return (ThreadHandle)data;
	}

	void WaitForThread(ThreadHandle handle)
	{
		int status;
		SDL_WaitThread(((SDL_ThreadData*)handle)->Handle, &status);
		delete (SDL_ThreadData*)handle;
	}

	void Sleep(int ms)
	{
		SDL_Delay((Uint32)ms);
	}
}
