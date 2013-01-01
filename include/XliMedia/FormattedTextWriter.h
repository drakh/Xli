#ifndef __XLI_FORMATTED_TEXT_WRITER_H__
#define __XLI_FORMATTED_TEXT_WRITER_H__

#include <Xli/Stream.h>
#include <Xli/String.h>

namespace Xli
{
	/**
		\ingroup Markup
	*/
	class FormattedTextWriter: public StreamWriter
	{
		int lineCount, indent;
		bool skipLine, disableSkip;
		CharString indentValue;
		
	public:
		FormattedTextWriter(Stream* stream, CharString indentValue = "\t");
		void WriteRaw(const char* str, int len);
		void SkipLine();
		void DisableSkip();
		void PushIndent();
		void PopIndent();
		void WriteIndent();
		void Write(const CharString& str);
		void EndLine();
		void BeginLine();
		void WriteLine(const CharString& str);
		void NewLine();
	};
}

#endif
