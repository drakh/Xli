#include <Xli/String.h>
#include <Xli/Exception.h>

namespace Xli
{
	int StringTools::HexToInt(const char* s)
	{
		int i;
		if (sscanf_s(s, "%x", &i) == 1) return i;
		if (sscanf_s(s, "%X", &i) == 1) return i;
		XLI_THROW_INVALID_FORMAT("Unable to convert string(hex) to integer");
	}

	bool StringTools::ToBool(const char* s)
	{
		if (!strcmp(s, "true")) return true;
		if (!strcmp(s, "false")) return false;
		if (!strcmp(s, "TRUE")) return true;
		if (!strcmp(s, "FALSE")) return false;
		if (!strcmp(s, "True")) return true;
		if (!strcmp(s, "False")) return false;
		XLI_THROW_INVALID_FORMAT("Unable to convert string to bool");
	}

	int StringTools::ToInt(const char* s)
	{
		int i;
		if (sscanf_s(s, "%d", &i) == 1) return i;
		XLI_THROW_INVALID_FORMAT("Unable to convert string to int");
	}

	float StringTools::ToFloat(const char* s)
	{
		float i;
		if (sscanf_s(s, "%f", &i) == 1) return i;
		if (sscanf_s(s, "%e", &i) == 1) return i;
		if (sscanf_s(s, "%E", &i) == 1) return i;
		XLI_THROW_INVALID_FORMAT("Unable to convert string to float");
	}

	double StringTools::ToDouble(const char* s)
	{
		double i;
		if (sscanf_s(s, "%lf", &i) == 1) return i;
		if (sscanf_s(s, "%le", &i) == 1) return i;
		if (sscanf_s(s, "%lE", &i) == 1) return i;
		XLI_THROW_INVALID_FORMAT("Unable to convert string to double");
	}

	uint StringTools::GetFloatLength(const char* str)
	{
		uint len = GetLength(str);
		while (len > 2 && str[len-1] == '0' && str[len-2] != '.') len--;
		return len;
	}

	CharString StringTools::Format(const char* format, va_list argList)
	{
		char buf[1024];
		int i = vsnprintf_s(buf, 1024, 1024, format, argList);
		if (i > 1024 || i < 0) XLI_THROW_INVALID_FORMAT("Result too long");
		return CharString(buf, i);
	}
}
