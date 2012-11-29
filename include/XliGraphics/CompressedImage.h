#ifndef __XLI_GRAPHICS_COMPRESSED_IMAGE_H__
#define __XLI_GRAPHICS_COMPRESSED_IMAGE_H__

#include <XliMedia/Image.h>
#include <Xli/Managed.h>
#include <Xli/Buffer.h>

namespace Xli
{
	class CompressedImage : public Image
	{
		int w, h, d;
		Format format;
		Shared<DataAccessor> data;

	public:
		CompressedImage(int width, int height, int depth, Format format, DataAccessor* data);

		virtual int GetWidth() const;
		virtual int GetHeight() const;
		virtual int GetDepth() const;
		virtual Format GetFormat() const;
		virtual Bitmap* ToBitmap();

		virtual unsigned int GetSizeInBytes() const;
		virtual const UInt8* GetData() const;
	};
}

#endif
