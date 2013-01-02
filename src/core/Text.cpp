#include <Xli/Text.h>
#include <Xli/Unicode.h>

namespace Xli
{
	CharString Text::LoadRaw(const String& fileName)
	{
		File f(fileName, FileModeRead);
		return TextReader(&f).ReadAllRaw();
	}

	Utf16String Text::Load(const String& fileName)
	{
		return Unicode::Utf8To16(LoadRaw(fileName));
	}

	void Text::Save(const String& fileName, const CharString& content)
	{
		File f(fileName, FileModeWrite);
		TextWriter(&f).Write(content);
	}

	void Text::Save(const String& fileName, const Utf16String& content)
	{
		Save(fileName, Unicode::Utf16To8(content));
	}
}
