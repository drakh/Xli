#include <Xli/Text.h>

namespace Xli
{
	String Text::Load(const String& filename)
	{
		File f(filename, FileModeRead);
		return TextReader(&f).ReadAll();
	}

	void Text::Save(const String& filename, const String& content)
	{
		File f(filename, FileModeWrite);
		TextWriter(&f).Write(content);
	}
}
