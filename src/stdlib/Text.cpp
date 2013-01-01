#include <Xli/Text.h>

namespace Xli
{
	CharString Text::Load(const CharString& fileName)
	{
		File f(fileName, FileModeRead);
		return TextReader(&f).ReadAll();
	}

	void Text::LoadLines(const CharString& fileName, Array<CharString>& result)
	{
		File f(fileName, FileModeRead);
		TextReader tr(&f);
		while (!tr.AtEndOfFile()) result.Add(tr.ReadLine());
	}

	void Text::Save(const CharString& fileName, const CharString& content)
	{
		File f(fileName, FileModeWrite);
		TextWriter(&f).WriteString(content);
	}
}
