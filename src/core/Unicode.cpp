#include <Xli/Unicode.h>
#include <Xli/Console.h>
#include "3rdparty/ConvertUTF.h"
#include <cstring>

namespace Xli
{
    static const ConversionFlags Flags = lenientConversion;

    Utf16String Unicode::Utf8To16(const char* str, int len)
    {
        const UTF8* src_start = (const UTF8*)str;
        const UTF8* src_end = src_start + len;

        Utf16String result = Utf16String::Create(len);
        UTF16* dst_start = (UTF16*)result.data;
        UTF16* dst_end = dst_start + result.length;

        ConversionResult res = ConvertUTF8toUTF16(&src_start, src_end, &dst_start, dst_end, Flags);

        if (res != conversionOK)
            ErrorPrintLine("WARNING: Unicode conversion failed");

        // adjust length
        result.length = (int)(dst_start - (UTF16*)result.data);
        result.data[result.length] = 0;

        return result;
    }
    
    Utf16String Unicode::Utf8To16(const char* str)
    {
        return Utf8To16(str, (int)strlen(str));
    }
    
    Utf16String Unicode::Utf8To16(const String& str)
    {
        return Utf8To16(str.DataPtr(), str.Length());
    }

    String Unicode::Utf16To8(const Utf16* str, int len)
    {
        const UTF16* src_start = (const UTF16*)str;
        const UTF16* src_end = src_start + len;

        String result = String::Create(len * 2);
        UTF8* dst_start = (UTF8*)result.data;
        UTF8* dst_end = dst_start + result.length;

        ConversionResult res = ConvertUTF16toUTF8(&src_start, src_end, &dst_start, dst_end, Flags);

        if (res != conversionOK)
            ErrorPrintLine("WARNING: Unicode conversion failed");

        // adjust length
        result.length = (int)(dst_start - (UTF8*)result.data);
        result.data[result.length] = 0;

        return result;
    }

    String Unicode::Utf16To8(const Utf16* str)
    {
        return Utf16To8(str, Utf16StrLen(str));
    }
    
    String Unicode::Utf16To8(const Utf16String& str)
    {
        return Utf16To8(str.DataPtr(), str.Length());
    }
}
