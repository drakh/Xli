#include <Xli/Time.h>
#include <Xli/Exception.h>
#include <SDL.h>

namespace Xli
{
	double GetTime()
	{
		return (double)SDL_GetTicks() * 0.001;
	}

	Timestamp GetTimestamp()
	{
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}
}
