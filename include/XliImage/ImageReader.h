#ifndef __XLI_MEDIA_IMAGE_READER_H__
#define __XLI_MEDIA_IMAGE_READER_H__

#include <XliImage/Format.h>
#include <XliImage/ProgressCallback.h>
#include <XliImage/Bitmap.h>

namespace Xli
{
    /**
        \ingroup XliImage
    */
    class ImageReader: public Object
    {
    public:
        virtual ~ImageReader() {}

        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
        virtual int GetDepth() = 0;
        virtual int GetBufferSize() = 0;
        virtual Format GetFormat() = 0;

        virtual void Read(void* targetBuffer, ProgressCallback* callback = 0) = 0;

        virtual Buffer* ReadBuffer(ProgressCallback* callback = 0);
        virtual Bitmap* ReadBitmap(ProgressCallback* callback = 0);
    };
}

#endif
