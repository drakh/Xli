#include <Xli/Text.h>

namespace Xli
{
	CharString Text::Load(const CharString& fileName)
	{
		File f(fileName, FileModeRead);
		return TextReader(&f).ReadAll();
	}

	List<CharString> Text::LoadLines(const CharString& fileName)
	{
		File f(fileName, FileModeRead);
		TextReader tr(&f);
		List<CharString> lines;
		while (!tr.AtEndOfFile()) lines.Add(tr.ReadLine());
		return lines;
	}

	void Text::Save(const CharString& fileName, const CharString& content)
	{
		File f(fileName, FileModeWrite);
		TextWriter(&f).WriteString(content);
	}
}
