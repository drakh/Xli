#include <XliGraphics/CompressedImage.h>

namespace Xli
{
	CompressedImage::CompressedImage(int w, int h, Format format, DataAccessor* data)
	{
		this->w = w;
		this->h = h;
		this->format = format;
		this->data = data;
	}

	int CompressedImage::GetWidth() const
	{
		return w;
	}

	int CompressedImage::GetHeight() const
	{
		return h;
	}

	Format CompressedImage::GetFormat() const
	{
		return format;
	}

	Bitmap* CompressedImage::ToBitmap()
	{
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	unsigned int CompressedImage::GetSizeInBytes() const
	{
		return data->GetSizeInBytes();
	}

	const UInt8* CompressedImage::GetData() const
	{
		return data->GetData();
	}
}
