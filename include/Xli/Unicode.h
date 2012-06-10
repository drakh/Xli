#ifndef __XLI_UTF8_H__
#define __XLI_UTF8_H__

#include <Xli/BaseTypes.h>
#include <Xli/String.h>

namespace Xli
{
	class Unicode
	{
	public:
		static bool IsLegalUtf8(const CharString& str);

		static Utf16String Utf8To16(const CharString& str);
		static Utf32String Utf8To32(const CharString& str);

		static CharString Utf16To8(const Utf16String& str);
		static Utf32String Utf16To32(const Utf16String& str);

		static CharString Utf32To8(const Utf32String& str);
		static Utf16String Utf32To16(const Utf32String& str);
	};
}

#endif
