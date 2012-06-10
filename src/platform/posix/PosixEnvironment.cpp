#include <cstdlib>
#include <XliPlatform/Environment.h>

namespace Xli
{
	bool Environment::GetVariable(const CharString& name, CharString& value)
	{
		char* result = getenv(name.Data());
		if (result)
		{
			value = result;
			return true;
		}
		return false;
	}

	bool Environment::SetVariable(const CharString& name, const CharString& value, bool overwrite)
	{
		return setenv(name.Data(), value.Data(), overwrite) == 0;
	}
}
