#ifndef __XLI_DATE_TIME_H__
#define __XLI_DATE_TIME_H__

#include <Xli/IntTypes.h>
#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliCoreTime
    */
    class DateTime
    {
    public:
        static const UInt64 PerMillisecond = 10000;
        static const UInt64 PerSecond = PerMillisecond * 1000;
        static const UInt64 PerMinute = PerSecond * 60;
        static const UInt64 PerHour = PerMinute * 60;
        static const UInt64 PerDay = PerHour * 24;

        static bool IsLeapYear(int year);
        static int GetDaysInYear(int year);
        static int GetDaysInMonth(int month, int year);

        int Year;
        int Month;
        int Day;
        int Hour;
        int Minute;
        int Second;
        int Millisecond;

        DateTime();
        DateTime(Timestamp timestamp);

        static DateTime Now();
        static DateTime NowUtc();
        static DateTime FromString(const String& str);

        Timestamp ToTimestamp() const;
        String ToString() const;

        DateTime operator - (const DateTime& dt) const;
        DateTime operator + (const DateTime& dt) const;
        
        DateTime& operator = (const DateTime& dt);
    };

}

#endif
