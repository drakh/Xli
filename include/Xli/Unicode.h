#ifndef __XLI_UNICODE_H__
#define __XLI_UNICODE_H__

#include <Xli/String.h>
#include <Xli/Utf16String.h>

namespace Xli
{
	/**
		\ingroup Core
	*/
	class Unicode
	{
	public:
		static Utf16String Utf8To16(const char* str, int len);
		static Utf16String Utf8To16(const char* str);
		static Utf16String Utf8To16(const String& str);

		static String Utf16To8(const Utf16* str, int len);
		static String Utf16To8(const Utf16* str);
		static String Utf16To8(const Utf16String& str);
	};
}

#endif
