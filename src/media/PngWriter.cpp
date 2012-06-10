#include <Xli/File.h>
#include <XliMedia/Png.h>
#include <XliMedia/ImageWriter.h>
#include <png.h>

namespace Xli
{
	class PngWriter: public ImageWriter
	{
		Shared<Stream> f;

		Format format;
		png_structp png_ptr;
		png_infop info_ptr;
		int comps;

		static void write(png_structp png_ptr, png_bytep data, png_size_t length)
		{
			((Stream*)png_get_io_ptr(png_ptr))->Write(data, 1, (int)length);
		}
		static void flush(png_structp png_ptr)
		{
			((Stream*)png_get_io_ptr(png_ptr))->Flush();
		}
		static void error(png_structp png_ptr, png_const_charp)
		{
			XLI_THROW("Error while writing to PNG file");
		}

	public:
		PngWriter(Stream* output, int width, int height, Format format)
		{
			f = output;

			png_byte color_type;
			const png_byte bit_depth = 8;
			this->format = format;

			switch (format)
			{
			case FormatL_8_UInt_Normalize:
				color_type = PNG_COLOR_TYPE_GRAY;
				comps = 1;
				break;
			case FormatLA_8_8_UInt_Normalize:
				color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
				comps = 2;
				break;
			case FormatRGB_8_8_8_UInt_Normalize:
				color_type = PNG_COLOR_TYPE_RGB;
				comps = 3;
				break;
			case FormatRGBA_8_8_8_8_UInt_Normalize:
				color_type = PNG_COLOR_TYPE_RGB_ALPHA;
				comps = 4;
				break;
			default:
				XLI_THROW(String("Unsupported PNG Format: ") + FormatInfo::ToString(format));
				break;
			}

			png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, error, NULL);
			if (!png_ptr) XLI_THROW("Error creating write struct");

			info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr) XLI_THROW("Error creating info struct");

			png_set_write_fn(png_ptr, (png_voidp)f.Get(), write, flush);

			png_set_IHDR(png_ptr, info_ptr, width, height,
				bit_depth, color_type, PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

			png_write_info(png_ptr, info_ptr);
		}
		
		virtual ~PngWriter()
		{
			png_destroy_write_struct(&png_ptr, &info_ptr);
		}

		virtual int GetWidth()
		{
			return png_get_image_width(png_ptr, info_ptr);
		}
		virtual int GetHeight()
		{
			return png_get_image_height(png_ptr, info_ptr);
		}
		virtual int GetDepth()
		{
			return 1;
		}
		virtual int GetBufferSize()
		{
			return comps * png_get_image_width(png_ptr, info_ptr) * png_get_image_height(png_ptr, info_ptr);
		}
		virtual Format GetFormat()
		{
			return format;
		}

		virtual void Write(void* sourceBuffer, int pitch, ProgressCallback* callback)
		{
			png_bytep row = (png_bytep)sourceBuffer;
			int height = png_get_image_height(png_ptr, info_ptr);

			for (int i = 0; i < height; i++)
			{
				if (callback) callback->Update(i, height);
				png_write_row(png_ptr, row);
				row += pitch;
			}

			png_write_end(png_ptr, NULL);
			if (callback) callback->Update(height, height);
		}
	};

	ImageWriter* Png::CreateWriter(Stream* output, int width, int height, Format format)
	{
		return new PngWriter(output, width, height, format);
	}

	void Png::Save(Stream* output, Bitmap* bmp)
	{
		Managed<ImageWriter> w = CreateWriter(output, bmp->GetWidth(), bmp->GetHeight(), bmp->GetFormat());
		w->WriteBitmap(bmp);
	}

	void Png::Save(const String& fileName, Bitmap* bmp)
	{
		File f(fileName, FileModeWrite);
		Save(&f, bmp);
	}
}
