#include <XliMedia/FormattedTextWriter.h>

namespace Xli
{
	FormattedTextWriter::FormattedTextWriter(Stream* stream, CharString indentValue): StreamWriter(stream)
	{
		this->indentValue = indentValue;
		indent = 0;
		lineCount = 1;
		skipLine = false;
		disableSkip = false;
	}
	void FormattedTextWriter::WriteRaw(const char* str, int len)
	{
		stream->WriteSafe(str, 1, len);
	}
	void FormattedTextWriter::SkipLine()
	{
		skipLine = true;
	}
	void FormattedTextWriter::DisableSkip()
	{
		disableSkip = true;
	}
	void FormattedTextWriter::PushIndent()
	{
		indent++;
	}
	void FormattedTextWriter::PopIndent()
	{
		indent--;
	}
	void FormattedTextWriter::WriteIndent()
	{
		for (int i = 0; i < indent; i++)
		{
			WriteRaw(indentValue.Data(), indentValue.Length());
		}
	}
	void FormattedTextWriter::Write(const CharString& str)
	{
		int start = 0;

		for (int i = 0; i < str.Length(); i++)
		{
			if (str[i] == '\n')
			{
				WriteRaw(str.Data() + start, 1 + i - start);
				start = i + 1;
				lineCount++; 
				WriteIndent();
			}
		}

		WriteRaw(str.Data() + start, str.Length() - start);
	}
	void FormattedTextWriter::EndLine()
	{
		WriteRaw("\n", 1);
		lineCount++;
	}
	void FormattedTextWriter::BeginLine()
	{
		if (skipLine && lineCount > 1 && !disableSkip)
		{
			skipLine = false;
			EndLine();
		}

		if (disableSkip)
		{
			skipLine = false;
			disableSkip = false;
		}

		WriteIndent();
	}
	void FormattedTextWriter::WriteLine(const CharString& str)
	{
		BeginLine();
		Write(str);
		EndLine();
	}
	void FormattedTextWriter::NewLine()
	{
		EndLine();
	}
}
