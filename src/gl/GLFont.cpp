#include <XliGL/GLFont.h>

namespace Xli
{
	GLFont::GLFont(BitmapFont* bf)
		: SpriteFont(bf)
	{
	    //Managed<Bitmap> bitmap = bf->Bitmap->ConvertTo(FormatRGBA_8_8_8_8_UInt_Normalize);
	    Managed<Bitmap> bitmap = bf->Bitmap;

		Texture = new GLTexture2D();
		Texture->SetData(bitmap, false);
		Texture->FilterNearest();
	}
}
