#ifndef __XLI_UNICODE_H__
#define __XLI_UNICODE_H__

#include <Xli/String.h>
#include <Xli/Utf16String.h>

namespace Xli
{
    /**
        \ingroup XliCoreText
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

        static Utf16 ToUpper(Utf16 chr);
        static Utf16 ToLower(Utf16 chr);
    private :
        static Utf16 ToUpperMap[65536];
        static Utf16 ToLowerMap[65536];
    };
}

#endif
