#ifndef __XLI_PLATFORM_TIME_H__
#define __XLI_PLATFORM_TIME_H__

#include <Xli/IntTypes.h>
#include <Xli/DateTime.h>

namespace Xli
{
    /**
        \addtogroup XliCoreTime
        @{
    */

    double GetSeconds();

    Timestamp GetTimestamp();
    Timestamp GetTimestampUtc();
    DateTime GetDatetime();

    /** @} */
}

#endif
