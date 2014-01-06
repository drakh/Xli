#include <XliMedia/ImageReader.h>
#include <Xli/File.h>

namespace Xli
{
    Buffer* ImageReader::ReadBuffer(ProgressCallback* callback)
    {
        Buffer* buf = Buffer::Create(GetBufferSize());
        Read(buf->Data(), callback);
        return buf;
    }

    Bitmap* ImageReader::ReadBitmap(ProgressCallback* callback)
    {
        Managed<Buffer> buf = ReadBuffer(callback);
        Bitmap* bmp = new Bitmap(GetWidth(), GetHeight(), GetFormat(), buf, GetWidth() * FormatInfo::SizeOf(GetFormat()), 0);
        return bmp;
    }
}
