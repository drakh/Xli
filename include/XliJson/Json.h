#ifndef __XLI_MEDIA_JSON_H__
#define __XLI_MEDIA_JSON_H__

#include <Xli/File.h>
#include <Xli/TextReader.h>
#include <XliJson/Value.h>

namespace Xli
{
    /**
        \ingroup XliJson
    */
    class Json
    {
    public:
        /**
            Formats a string to be outputted to a JSON document.
            Converts to Utf8, adds quotes and escapes special characters.
        */
        static String ToStringLiteral(const String& str);

        static Value Parse(const String& code, bool preserveOrder = false);

        static XLI_INLINE Value Load(Stream* stream, bool preserveOrder = false)
        {
            return Parse(TextReader(stream).ReadAll(), preserveOrder);
        }

        static XLI_INLINE Value Load(const String& filename, bool preserveOrder = false)
        {
            File f(filename, FileModeRead);
            return Load(&f, preserveOrder);
        }

        static void Save(Stream* stream, const Value& value);

        static XLI_INLINE void Save(const String& filename, const Value& value)
        {
            File f(filename, FileModeWrite);
            Save(&f, value);
        }

        static String Stringify(const Value& value);
    };
}

#endif
