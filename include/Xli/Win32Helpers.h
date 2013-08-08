#ifndef __XLI_PLATFORM_WIN32_HELPERS_H__
#define __XLI_PLATFORM_WIN32_HELPERS_H__

#include <Xli/String.h>

namespace Xli
{
	/**
		\ingroup XliCorePlatformSpecific
	*/
	class Win32Helpers
	{
	public:
		/**
			Returns a String describing the error code returned from ::GetLastError()
		*/
		static String GetLastErrorString();
	};
}

#endif
