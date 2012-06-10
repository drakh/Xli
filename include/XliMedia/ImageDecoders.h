#ifndef __XLI_IMAGE_DECODERS_H__
#define __XLI_IMAGE_DECODERS_H__

#include <XliMedia/Format.h>
#include <Xli/Map2D.h>

namespace Xli
{
	Bitmap2D* DecodeToBitmap2D(Format format, int width, int height, Buffer* source);
	Bitmap2D* DecodeToBitmap2DFromRGBA_8_8_8_8_UInt_Normalize(int width, int height, Buffer* buf);
	Bitmap2D* DecodeToBitmap2DFromRGB_8_8_8_UInt_Normalize(int width, int height, Buffer* buf);
	Bitmap2D* DecodeToBitmap2DFromL_8_UInt_Normalize(int width, int height, Buffer* buf);

	Bitmap2D* DecodeToBitmap2DFromBC1(int width, int height, Buffer* buf);
}

#endif
