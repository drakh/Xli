#ifndef __XLI_PLATFORM_TIME_H__
#define __XLI_PLATFORM_TIME_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
    /**
        \addtogroup XliCoreTime
        @{
    */

    double GetSeconds();

    /// TODO: Deprecated function, use GetSeconds()
    XLI_INLINE double GetTime() { return GetSeconds(); }

    Timestamp GetTimestamp();
    Timestamp GetTimestampUtc();

    /** @} */
}

#endif
