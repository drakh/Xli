#include <Xli/DateTime.h>
#include <Xli/Time.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        Timestamp ConvertToTimestamp(time_t time)
        {
            return (Timestamp)time * DateTime::PerSecond + (369 * 365 + 89) * DateTime::PerDay;
        }
    }

    double GetSeconds()
    {
        struct timeval t;
        gettimeofday(&t, 0);
        return (double)t.tv_sec + (double)t.tv_usec * (1.0 / 1000000.0);
    }

    Timestamp GetTimestamp()
    {
        time_t t;
        time(&t);
        return PlatformSpecific::ConvertToTimestamp(mktime(localtime(&t)));
    }

    Timestamp GetTimestampUtc()
    {
        time_t t;
        time(&t);
        return PlatformSpecific::ConvertToTimestamp(mktime(gmtime(&t)));
    }
}
