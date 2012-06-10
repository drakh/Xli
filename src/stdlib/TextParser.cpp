#include <Xli/TextParser.h>

namespace Xli
{
	TextParser::TextParser(const char* data, int len)
	{
		this->data = data;
		this->len = len;
		this->start = 0;
	}

	bool TextParser::AtEnd() const
	{
		return start >= len;
	}

	bool TextParser::Match(const char* head) const
	{
		int head_len = strlen(head);
		if (start + head_len > len) return false;
		for (int i = 0; i < head_len; i++) if (head[i] != data[start + i]) return false;
		return true;
	}

	void TextParser::SkipSpace()
	{
		while (start < len) if (isspace(data[start])) start++; else break;
	}

	void TextParser::SkipToken()
	{
		while (start < len) if (!isspace(data[start])) start++; else break;
		while (start < len) if (isspace(data[start])) start++; else break;
	}

	void TextParser::SkipLine()
	{
		while (start < len) if (data[start] != '\n') start++; else break;
		while (start < len) if (isspace(data[start])) start++; else break;
	}

	int TextParser::ParseInt()
	{
		int i = -1;
		sscanf_s(data + start, "%d", &i);
		SkipToken();
		return i;
	}

	CharString TextParser::NextToken()
	{
		int a = start;
		SkipToken();
		if (start <= a) return "";
		int b = start; while (b > a && isspace(data[b - 1])) b--;
		return CharString(data + a, b - a);
	}

	CharString TextParser::NextLine()
	{
		int a = start;
		SkipLine();
		if (start <= a) return "";
		int b = start; while (b > a && isspace(data[b - 1])) b--;
		return CharString(data + a, b - a);
	}

	int TextParser::ParseDataToInt(const char* lineStart)
	{
		SkipSpace();
		while (!AtEnd())
		{
			if (Match(lineStart))
			{
				SkipToken();
				return ParseInt();
			}
			SkipLine();
		}
		return -1;
	}

	CharString TextParser::ParseDataToLine(const char* lineStart)
	{
		SkipSpace();
		while (!AtEnd())
		{
			if (Match(lineStart))
			{
				SkipToken();
				return NextLine();
			}
			SkipLine();
		}
		return "";
	}

	CharString TextParser::ParseDataToToken(const char* lineStart)
	{
		SkipSpace();
		while (!AtEnd())
		{
			if (Match(lineStart))
			{
				SkipToken();
				return NextToken();
			}
			SkipLine();
		}
		return "";
	}
}
