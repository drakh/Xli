#ifndef __XLI_MEDIA_PNG_H__
#define __XLI_MEDIA_PNG_H__

#include <XliMedia/ImageReader.h>
#include <XliMedia/ImageWriter.h>

namespace Xli
{
	/**
		\ingroup Textures
	*/
	class Png
	{
	public:
		static ImageWriter* CreateWriter(Stream* output, int width, int height, Format format);
		static ImageReader* CreateReader(Stream* input);

		static Bitmap* Load(Stream* input);
		static Bitmap* Load(const String& fileName);

		static void Save(Stream* output, Bitmap* bmp);
		static void Save(const String& fileName, Bitmap* bmp);
	};
}

#endif
