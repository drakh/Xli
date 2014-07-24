#ifndef __XLI_UNICODE_H__
#define __XLI_UNICODE_H__

#include <Xli/String.h>
#include <Xli/Utf16String.h>

namespace Xli
{
    /**
        \ingroup XliText
    */
    enum UnicodeFlags
    {
        UnicodeFlagsModifiedUtf8 = 1 << 0,
        UnicodeFlagsIgnoreError = 1 << 1,
    };

    /**
        \ingroup XliText
    */
    class Unicode
    {
    public:
        static bool IsLegalUtf8(const char* str, int len, int flags = 0);
        static bool IsLegalUtf8(const String& str, int flags = 0);

        static Utf16String Utf8To16(const char* str, int len, int flags = 0);
        static Utf16String Utf8To16(const String& str, int flags = 0);

        static String Utf16To8(const Utf16* str, int len, int flags = 0);
        static String Utf16To8(const Utf16String& str, int flags = 0);

        static Utf16 ToUpper(Utf16 chr);
        static Utf16 ToLower(Utf16 chr);
    };
}

#endif
