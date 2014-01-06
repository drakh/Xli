#ifndef __XLI_TEXT_WRITER_H__
#define __XLI_TEXT_WRITER_H__

#include <Xli/Stream.h>
#include <Xli/String.h>
#include <Xli/File.h>
#include <Xli/ToString.h>

namespace Xli
{
    /**
        \ingroup XliCoreIO
    */
    class TextWriter: public StreamWriter
    {
    public:
        TextWriter(Stream* stream);

        void Write(const char* str, int len);
        void WriteFormat(const char* format, ...);
        void Write(const char* str);
        void Write(const String& str);
        void WriteLine();

        template <typename T> void Write(const T& t)
        {
            Write(Xli::ToString(t));
        }

        template <typename T> void WriteLine(const T& t)
        {
            Write(t);
            WriteLine();
        }
    };
}

#endif
