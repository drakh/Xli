#ifndef __XLI_IMAGE_ENCODERS_H__
#define __XLI_IMAGE_ENCODERS_H__

#include <XliMedia/Format.h>
#include <Xli/Map2D.h>

namespace Xli
{
	Buffer* EncodeFromBitmap2D(Bitmap2D* bmp, Format format);
	Buffer* EncodeFromBitmap2DToRGBA_8_8_8_8_UInt(Bitmap2D* bmp);
	Buffer* EncodeFromBitmap2DToRGB_8_8_8_UInt(Bitmap2D* bmp);
	Buffer* EncodeFromBitmap2DToL_8_UInt(Bitmap2D* bmp);

	/**
		Encodes a bitmap to block-compressed (BC1) format.
		@param bmp The bitmap to encode
		@param writtenWidth Receives the width of the encoded image, in pixels (the width of the bitmap rounded up to nearest 4 pixels).
		@param writtenHeight Receives the height of the encoded image, in pixels (the height of the bitmap rounded up to nearest 4 pixels).
		@return A buffer containing the compressed blocks.
	*/
	Buffer* EncodeFromBitmap2DToBC1(Bitmap2D* bmp, int& writtenWidth, int& writtenHeight);

}


#endif
