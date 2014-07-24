#ifndef __XLI_MEDIA_PNG_H__
#define __XLI_MEDIA_PNG_H__

#include <XliImage/ImageReader.h>
#include <XliImage/ImageWriter.h>
#include <Xli/Stream.h>

namespace Xli
{
    /**
        \ingroup XliImage
    */
    class Png
    {
    public:
        static ImageWriter* CreateWriter(Stream* output, int width, int height, Format format);
        static ImageReader* CreateReader(Stream* input);

        static Bitmap* Load(Stream* input);
        static Bitmap* Load(const String& filename);

        static void Save(Stream* output, Bitmap* bmp);
        static void Save(const String& filename, Bitmap* bmp);
    };
}

#endif
