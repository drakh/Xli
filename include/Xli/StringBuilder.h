#ifndef __XLI_STRING_BUILDER_H__
#define __XLI_STRING_BUILDER_H__

#include "String.h"
#include "Array.h"

namespace Xli
{
    /**
        \ingroup XliCoreText
    */
    class StringBuilder
    {
        Array<char> buffer;

    public:
        StringBuilder(int cap = 1024);

        StringBuilder& operator += (const String& text);
        StringBuilder& operator += (const char* str);
        StringBuilder& operator += (char c);

        int GetLength() const;

        void Append(const char* str);
        void Append(const String& text);
        void Append(char c);

        void AppendLine(const char* str);
        void AppendLine(const String& text);
        void AppendLine();

        void AppendFormat(const char* format, ...);

        void Clear();

        String GetString() const;
        operator String() const;
    };
}

#endif
