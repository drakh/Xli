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

        Timestamp ConvertToTimestamp(const tm* const time)
        {
            DateTime dt;
            dt.Year = time->tm_year;
            dt.Month = time->tm_mon;
            dt.Day = time->tm_mday;
            dt.Hour = time->tm_hour;
            dt.Minute = time->tm_min;
            dt.Second = time->tm_sec;
            return dt.ToTimestamp();
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
        return PlatformSpecific::ConvertToTimestamp(localtime(&t));
    }

    Timestamp GetTimestampUtc()
    {
        time_t t;
        time(&t);
        return PlatformSpecific::ConvertToTimestamp(gmtime(&t));
    }
}
