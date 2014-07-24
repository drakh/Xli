#ifndef __XLI_MEDIA_JPEG_H__
#define __XLI_MEDIA_JPEG_H__

#include <XliImage/ImageReader.h>
#include <XliImage/ImageWriter.h>
#include <Xli/Stream.h>

namespace Xli
{
    /**
        \ingroup XliImage
    */
    class Jpeg
    {
    public:
        static ImageWriter* CreateWriter(Stream* output, int width, int height, Format format, int quality);
        static ImageReader* CreateReader(Stream* input);

        static Bitmap* Load(Stream* input);
        static Bitmap* Load(const String& filename);

        static void Save(Stream* output, Bitmap* bmp, int quality = 75);
        static void Save(const String& filename, Bitmap* bmp, int quality = 75);
    };
}

#endif
