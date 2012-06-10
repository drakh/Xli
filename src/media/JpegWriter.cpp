#include <Xli/File.h>
#include <XliMedia/Jpeg.h>
#include <XliMedia/ImageWriter.h>

extern "C"
{
	#include <jpeglib.h>
}

namespace Xli
{
	class JpegWriter: public ImageWriter
	{
		static const int BufSize = 4096;

		struct ClientData
		{
			Shared<Stream> File;
			UInt8 Buf[BufSize];
			ClientData(Stream* f): File(f) {}
		};

		ClientData cd;
		Format format;

		jpeg_compress_struct cinfo;
		jpeg_destination_mgr jdest;
		jpeg_error_mgr pub;

		static void initDestination(j_compress_ptr cinfo)
		{
			ClientData* cd = reinterpret_cast<ClientData*>(cinfo->client_data);
			struct jpeg_destination_mgr *jdest = cinfo->dest;

			jdest->next_output_byte = cd->Buf;
			jdest->free_in_buffer = BufSize;
		}
		static boolean emptyOutputBuffer(j_compress_ptr cinfo)
		{
			ClientData* cd = reinterpret_cast<ClientData*>(cinfo->client_data);
			struct jpeg_destination_mgr *jdest = cinfo->dest;

			cd->File->Write(cd->Buf, 1, BufSize);

			jdest->next_output_byte = cd->Buf;
			jdest->free_in_buffer = BufSize;

			return TRUE;
		}
		static void termDestination(j_compress_ptr cinfo)
		{
			ClientData* cd = reinterpret_cast<ClientData*>(cinfo->client_data);
			struct jpeg_destination_mgr *jdest = cinfo->dest;

			int d = BufSize - jdest->free_in_buffer;
			if (d) cd->File->Write(cd->Buf, 1, d);

			cd->File->Flush();
		}
		static void errorExit(j_common_ptr cinfo)
		{
			char buf[JMSG_LENGTH_MAX];
			(*cinfo->err->format_message) (cinfo, buf);
			XLI_THROW(String("Error while writing JPEG file: ") + buf);
		}

	public:
		JpegWriter(Stream* output, int width, int height, Format format, int quality): cd(output)
		{
			jpeg_create_compress(&cinfo);

			switch (format)
			{
			case FormatL_8_UInt_Normalize:
				cinfo.input_components = 1;
				cinfo.in_color_space = JCS_GRAYSCALE;
				break;
			case FormatRGB_8_8_8_UInt_Normalize: 
				cinfo.input_components = 3;
				cinfo.in_color_space = JCS_RGB;
				break;
			default:
				XLI_THROW(String("Unsupported JPEG Format: ") + FormatInfo::ToString(format));
				break;
			}

			this->format = format;
			cinfo.image_width = width;
			cinfo.image_height = height;

			cinfo.err = jpeg_std_error(&pub);
			cinfo.dest = &jdest;
			cinfo.client_data = (void*)&cd;

			jdest.next_output_byte = 0;
			jdest.free_in_buffer = 0;
			jdest.init_destination = initDestination;
			jdest.empty_output_buffer = emptyOutputBuffer;
			jdest.term_destination = termDestination;

			pub.error_exit = errorExit;

			jpeg_set_defaults(&cinfo);
			jpeg_set_quality(&cinfo, quality, TRUE);
		}
		virtual ~JpegWriter()
		{
			jpeg_destroy_compress(&cinfo);
		}

		virtual int GetWidth()
		{
			return cinfo.image_width;
		}
		virtual int GetHeight()
		{
			return cinfo.image_height;
		}
		virtual int GetDepth()
		{
			return 1;
		}
		virtual int GetBufferSize()
		{
			return cinfo.image_width * cinfo.image_height * cinfo.input_components;
		}
		virtual Format GetFormat()
		{
			return format;
		}

		virtual void Write(void* sourceBuffer, int pitch, ProgressCallback* callback)
		{
			jpeg_start_compress(&cinfo, TRUE);
			JSAMPROW j = (JSAMPROW)sourceBuffer;

			while (cinfo.next_scanline < cinfo.image_height)
			{
				if (callback) callback->Update(cinfo.next_scanline, cinfo.image_height);
				jpeg_write_scanlines(&cinfo, &j, 1);
				j += pitch;
			}

			jpeg_finish_compress(&cinfo);
			if (callback) callback->Update(cinfo.image_height, cinfo.image_height);
		}
	};

	ImageWriter* Jpeg::CreateWriter(Stream* output, int width, int height, Format format, int quality)
	{
		return new JpegWriter(output, width, height, format, quality);
	}

	void Jpeg::Save(Stream* output, Bitmap* bmp, int quality)
	{
		Managed<ImageWriter> w = CreateWriter(output, bmp->GetWidth(), bmp->GetHeight(), bmp->GetFormat(), quality);
		w->WriteBitmap(bmp);
	}
	void Jpeg::Save(const String& fileName, Bitmap* bmp, int quality)
	{
		File f(fileName, FileModeWrite);
		Save(&f, bmp, quality);
	}
}
