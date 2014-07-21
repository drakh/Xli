#ifndef __XLI_PLATFORM_TIME_H__
#define __XLI_PLATFORM_TIME_H__

#include <Xli/IntTypes.h>

namespace Xli
{
    /**
        \addtogroup XliTime
        @{
    */

    double GetSeconds();

    Timestamp GetTimestamp();
    Timestamp GetTimestampUtc();

    /** @} */
}

#endif
