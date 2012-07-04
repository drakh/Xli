#include <XliPlatform/Time.h>
#include <XliPlatform/Win32Windows.h>

namespace Xli
{
	double GetTime()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		return (double)currentTime.QuadPart / (double)frequency.QuadPart;
	}

	Timestamp GetTimestamp()
	{
		FILETIME utcft, ft;
		GetSystemTimeAsFileTime(&utcft);
		FileTimeToLocalFileTime(&utcft, &ft);
		return ((Timestamp)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
	}
}
