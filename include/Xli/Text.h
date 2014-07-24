#ifndef __XLI_TEXT_H__
#define __XLI_TEXT_H__

#include <Xli/TextReader.h>
#include <Xli/TextWriter.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    class Text
    {
    public:
        static String Load(const String& filename);
        static String Load(Stream* stream);
        
        static void Save(const String& filename, const String& content);
        static void Save(Stream* stream, const String& content);
    };
}

#endif
