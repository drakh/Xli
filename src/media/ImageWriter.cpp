#include <XliMedia/ImageWriter.h>
#include <Xli/File.h>

namespace Xli
{
    void ImageWriter::WriteBitmap(Bitmap* bmp, ProgressCallback* callback)
    {
        if (bmp->GetWidth() != GetWidth()) XLI_THROW_INVALID_ARGUMENT("Bitmap must have same width as ImageWriter");
        if (bmp->GetHeight() != GetHeight()) XLI_THROW_INVALID_ARGUMENT("Bitmap must have same height as ImageWriter");
        if (bmp->GetFormat() != GetFormat()) XLI_THROW_INVALID_ARGUMENT("Bitmap must have same format as ImageWriter");
        Write(bmp->GetDataPtr(), bmp->GetPitch(), callback);
    }

    void ImageWriter::WriteImage(Image* img, ProgressCallback* callback)
    {
        Managed<Bitmap> bmp = img->ToBitmap();
        WriteBitmap(bmp, callback);
    }
}
