#include <Xli.h>
#include <XliMedia.h>

using namespace Xli;

static void LoadFontFace(const String& filename, int size)
{
	PrintLine("------------------------------------------------");
	PrintLine("Loading Font Face: " + filename + ", size = " + size);
	PrintLine("------------------------------------------------");

	Managed<Stream> stream = Assets->OpenFile(filename);
	Managed<FontFace> font = FreeType::LoadFontFace(stream, size);

	PrintLine((String)"Ascender: " + font->GetAscender());
	PrintLine((String)"Descender: " + font->GetDescender());
	PrintLine((String)"LineHeight: " + font->GetLineHeight());

	for (int i = 32; i < 127; i++)
	{
		Managed<Bitmap> bmp = font->RenderGlyph(i, FontRenderModeNormal);
		PrintLine(String(i) + " (" + (char)i + "): " + font->GetAdvance(i).ToString() + "; " + font->GetBearing(i).ToString() + "; " + bmp->GetWidth() + "x" + bmp->GetHeight());
	}
}

int Main(const Array<String>& args)
{
	LoadFontFace("arial.ttf", 100);
	return 0;
}
