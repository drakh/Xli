#ifndef __XLI_PLATFORM_TIME_H__
#define __XLI_PLATFORM_TIME_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
	/**
		\addtogroup XliCoreTime
		@{
	*/

	double GetTime();

	Timestamp GetTimestamp();
	Timestamp GetTimestampUtc();

	/** @} */
}

#endif
