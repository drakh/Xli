#ifndef __XLI_FORMATTED_TEXT_WRITER_H__
#define __XLI_FORMATTED_TEXT_WRITER_H__

#include <Xli/Stream.h>
#include <Xli/String.h>

namespace Xli
{
    /**
        \ingroup XliJson
    */
    class FormattedTextWriter: public StreamWriterBase
    {
        int lineCount, indent;
        bool skipLine, disableSkip;
        String indentValue;
        
    public:
        FormattedTextWriter(Stream* stream, String indentValue = "\t");
        void WriteRaw(const char* str, int len);
        void SkipLine();
        void DisableSkip();
        void PushIndent();
        void PopIndent();
        void WriteIndent();
        void Write(const String& str);
        void EndLine();
        void BeginLine();
        void WriteLine(const String& str);
        void NewLine();
    };
}

#endif
