#include <Xli/Text.h>

namespace Xli
{
	String Text::Load(const String& fileName)
	{
		File f(fileName, FileModeRead);
		return TextReader(&f).ReadAll();
	}

	void Text::Save(const String& fileName, const String& content)
	{
		File f(fileName, FileModeWrite);
		TextWriter(&f).Write(content);
	}
}
