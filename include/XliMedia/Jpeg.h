#ifndef __XLI_MEDIA_JPEG_H__
#define __XLI_MEDIA_JPEG_H__

#include <XliMedia/ImageReader.h>
#include <XliMedia/ImageWriter.h>
#include <Xli/Stream.h>

namespace Xli
{
	class Jpeg
	{
	public:
		static ImageWriter* CreateWriter(Stream* output, int width, int height, Format format, int quality);
		static ImageReader* CreateReader(Stream* input);
		static Bitmap* Load(Stream* input);
		static Bitmap* Load(const String& fileName);
		static void Save(Stream* output, Bitmap* bmp, int quality = 75);
		static void Save(const String& fileName, Bitmap* bmp, int quality = 75);
	};
}

#endif
