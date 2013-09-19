#include <Xli.h>
#include <XliMedia.h>

using namespace Xli;

static void LoadFontFace(const String& filename, float size)
{
	PrintLine("------------------------------------------------");
	PrintLine("Loading Font Face: " + filename + ", size = " + size);
	PrintLine("------------------------------------------------");

	Managed<Stream> stream = Assets->OpenFile(filename);
	Managed<FontFace> font = FreeType::LoadFontFace(stream);

	PrintLine((String)"Ascender: " + font->GetAscender(size));
	PrintLine((String)"Descender: " + font->GetDescender(size));
	PrintLine((String)"LineHeight: " + font->GetLineHeight(size));

	for (int i = 32; i < 127; i++)
	{
		if (font->ContainsGlyph(size, i))
		{
			Vector2 advance, bearing;
			Managed<Bitmap> bmp = font->RenderGlyph(size, i, FontRenderModeNormal, &advance, &bearing);
			PrintLine(String(i) + " (" + (char)i + "): " + advance.ToString() + "; " + bearing.ToString() + "; " + bmp->GetWidth() + "x" + bmp->GetHeight());
		}
	}
}

int Main(const Array<String>& args)
{
	LoadFontFace("arial.ttf", 100);
	return 0;
}
