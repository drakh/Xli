#ifndef __XLI_PLATFORM_ENVIRONMENT_H__
#define __XLI_PLATFORM_ENVIRONMENT_H__

#include <Xli/String.h>

namespace Xli
{
	class Environment
	{
	public:
		static bool GetVariable(const CharString& name, CharString& value);
		static bool SetVariable(const CharString& name, const CharString& value, bool overwrite = true);
	};
}

#endif
