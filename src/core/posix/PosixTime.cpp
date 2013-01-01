#include <Xli/DateTime.h>
#include <XliPlatform/Time.h>

#include <sys/time.h>
#include <time.h>

namespace Xli
{
	double GetTime()
	{
		struct timeval t;
		gettimeofday(&t, 0);
		return (double)t.tv_sec + (double)t.tv_usec * (1.0 / 1000000.0);
	}

	Timestamp GetTimestamp()
	{
		time_t now;
		time(&now);
		return (Timestamp)now * DateTime::PerSecond + (369 * 365 + 89) * DateTime::PerDay;
	}
}
