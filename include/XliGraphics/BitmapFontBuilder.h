#ifndef __XLI_BITMAP_FONT_BUILDER_H__
#define __XLI_BITMAP_FONT_BUILDER_H__

#include <XliMedia/FontFace.h>
#include "BitmapFont.h"

namespace Xli
{
	class BitmapFontBuilder
	{
	public:
		int Padding;
		bool AllowRotation;
		bool PowerOfTwo;
		Format PixelFormat;
		String CharSet;

		BitmapFontBuilder();

		BitmapFont* Build(FontFace* ff);
		BitmapFont* Build(const Xli::String& fileName, int size);
	};
}

#endif
