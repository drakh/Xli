#include <Xli/File.h>
#include <XliMedia/Png.h>
#include <XliMedia/ImageReader.h>
#include <png.h>

namespace Xli
{
	class PngReader: public ImageReader
	{
		Shared<Stream> f;

		png_structp png_ptr;
		png_infop info_ptr;

		UInt32 width, height;
		Format format;
		int comps;

		static void read(png_structp png_ptr, png_bytep data, png_size_t length)
		{
			((Stream*)png_get_io_ptr(png_ptr))->Read(data, 1, (int)length);
		}
		static void error(png_structp png_ptr, png_const_charp)
		{
			XLI_THROW("Error while reading from PNG file");
		}

	public:
		PngReader(Stream* input)
		{
			f = input;

			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, error, NULL);
			if (!png_ptr)
			{
				XLI_THROW("Not a PNG file");
			}

			info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr)
			{
				png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
				XLI_THROW("Not a PNG file");
			}

			png_set_read_fn (png_ptr, (png_voidp)f.Get(), read);

			png_read_info(png_ptr, info_ptr);

			int bit_depth = png_get_bit_depth (png_ptr, info_ptr);
			int color_type = png_get_color_type (png_ptr, info_ptr);

			if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
			else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);

			if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha (png_ptr);

			if (bit_depth == 16) png_set_strip_16 (png_ptr);
			else if (bit_depth < 8) png_set_packing (png_ptr);

			png_read_update_info (png_ptr, info_ptr);

			png_get_IHDR (png_ptr, info_ptr,
				(png_uint_32*)(&width),
				(png_uint_32*)(&height),
				&bit_depth, &color_type,
				NULL, NULL, NULL);

			switch (color_type)
			{
			case PNG_COLOR_TYPE_GRAY:
				format = FormatL_8_UInt_Normalize;
				comps = 1;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				format = FormatLA_8_8_UInt_Normalize;
				comps = 2;
				break;
			case PNG_COLOR_TYPE_RGB: 
				format = FormatRGB_8_8_8_UInt_Normalize; 
				comps = 3;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA: 
				format = FormatRGBA_8_8_8_8_UInt_Normalize; 
				comps = 4;
				break;
			default:
				XLI_THROW("Unsupported PNG pixel format");
				break;
			}
		}
		virtual ~PngReader()
		{
			png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
		}
		virtual int GetWidth()
		{
			return width;
		}
		virtual int GetHeight()
		{
			return height;
		}
		virtual int GetDepth()
		{
			return 1;
		}
		virtual Format GetFormat()
		{
			return format;
		}
		virtual int GetBufferSize()
		{
			return comps *width * height;
		}
		virtual void Read(void* targetBuffer, ProgressCallback* callback)
		{
			png_bytep row = (png_bytep)targetBuffer;
			int pitch = width * comps;

			for (unsigned int i = 0; i < height; ++i)
			{
				if (callback) callback->Update(i, height);
				png_read_row(png_ptr, row, NULL);
				row += pitch;
			}
			
			png_read_end(png_ptr, NULL);
			if (callback) callback->Update(height, height);
		}
	};

	ImageReader* Png::CreateReader(Stream* input)
	{
		return new PngReader(input);
	}

	Bitmap* Png::Load(Stream* input)
	{
		Managed<ImageReader> r = CreateReader(input);
		return r->ReadBitmap();
	}
	Bitmap* Png::Load(const String& fileName)
	{
		Managed<File> f = new File(fileName, FileModeRead);
		return Load(f);
	}
}
