#ifndef __XLI_MEDIA_BITMAP_H__
#define __XLI_MEDIA_BITMAP_H__

#include <Xli/Buffer.h>
#include <Xli/Vector2.h>
#include <Xli/Rectangle.h>
#include <XliMedia/Image.h>
#include <Xli/Color.h>

namespace Xli
{
	enum RotateAngle
	{
		RotateAngle90CW,
		RotateAngle90CCW,
		RotateAngle180,
	};

	class Bitmap: public Image
	{
		Format format;
		int width, height, pitch;
		DataAccessor* dataOwner;
		UInt8* data;

	public:
		Bitmap(int width, int height, Format format);
		Bitmap(int width, int height, Format format, DataAccessor* data, int pitch, int offset);
		virtual ~Bitmap();

		Format GetFormat() const;
		int GetWidth() const;
		int GetHeight() const;
		int GetPitch() const;
		int GetComponentCount() const;
		int GetBytesPerPixel() const;
		Recti GetRect() const;

		Coloru8 GetPixelColor(int x, int y);
		void SetPixelColor(int x, int y, const Coloru8& color);

		/**
			Returns a version of this bitmap where pixels are packed (pitch = pixelsize*width).
			If this bitmap is already packed, then a new reference is added and this is returned.
		*/
		Bitmap* Pack();
		Bitmap* ToBitmap();
		Bitmap* SubBitmap(const Recti& rect);
		Bitmap* ConvertTo(Format format);
		Bitmap* Rotate(RotateAngle angle);

		Bitmap* DownSample2x2();
		void GenerateMipMaps(Array<Bitmap*>& chain);

		void Clear();
		void Blit(Bitmap* src, Recti srcRect, Vector2i dstPos);

		virtual unsigned int GetSizeInBytes() const;
		
		UInt8* GetData();
		virtual const UInt8* GetData() const;

		UInt8* GetScanlinePtr(int y);
		const UInt8* GetScanlinePtr(int y) const;

		UInt8* GetPixelPtr(int x, int y);
		const UInt8* GetPixelPtr(int x, int y) const;

		DataAccessor* GetDataAccessor();
	};
}

#endif
