#include <XliGL/GLTexture2D.h>
#include <XliMedia/Png.h>

namespace Xli
{
	GLTexture2D::GLTexture2D()
	{
		w = h = mips = 1;
		glGenTextures(1, &handle);
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &handle);
	}

	void GLTexture2D::SetSize(int width, int height)
	{
		this->w = width;
		this->h = height;
	}

	static void XliToGLFormat(Format format, GLenum& glFormat, GLenum& glType)
	{
		switch (format)
		{
		case FormatRGBA_8_8_8_8_UInt_Normalize:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_BYTE;
			break;

		case FormatRGB_8_8_8_UInt_Normalize:
			glFormat = GL_RGB;
			glType = GL_UNSIGNED_BYTE;
			break;

		case FormatR_8_UInt_Normalize:
			//XLI_DEBUG_PRINT("WARNING: FormatR_8_UInt_Normalize treated as FormatL_8_UInt_Normalize");
	
		case FormatL_8_UInt_Normalize:
			glFormat = GL_LUMINANCE;
			glType = GL_UNSIGNED_BYTE;
			break;

		case FormatLA_8_8_UInt_Normalize:
			glFormat = GL_LUMINANCE_ALPHA;
			glType = GL_UNSIGNED_BYTE;
			break;

		default:
			XLI_THROW(String("Unsupported format: ") + FormatInfo::ToString(format));
		}
	}

	void GLTexture2D::SetData(GLuint handle, int width, int height, Format format, void* data, bool mipmaps, int* mips)
	{
		//PrintLine(FormatInfo::ToString(format) + (String)", " + width + ", " + height + ", " + mipmaps);

		GLenum glFormat;
		GLenum glType;

		XliToGLFormat(format, glFormat, glType);

		glBindTexture(GL_TEXTURE_2D, handle);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
#ifdef XLI_GLES2
      
        if (mipmaps && !(IsPow2(width) && IsPow2(height)))
        {
            ErrorPrintLine("GL WARNING: Cannot upload mipmaps on NPOT textures; disabling mipmaps");
            mipmaps = false;
        }
        
#endif
        
		if (mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			BufferPointer bp(data, width*height*FormatInfo::SizeOf(format), false);
			Managed<Bitmap> mip = new Bitmap(width, height, format, &bp, width*FormatInfo::SizeOf(format), 0);

			if (mips != 0) *mips = 0;
			
			for (int i = 0; ; i++)
			{
				glTexImage2D(GL_TEXTURE_2D, i, glFormat, mip->GetWidth(), mip->GetHeight(), 0, glFormat, glType, mip->GetData());
				if (mip->GetWidth() == 1 && mip->GetHeight() == 1) break;
				mip = mip->DownSample2x2();
				if (mips != 0) (*mips)++;
			}
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, glType, data);
		}

		GLCheckError();
	}

	void GLTexture2D::SetData(GLuint handle, Texture* tex, int* width, int* height, int* mipCount)
	{
		if (width) *width = tex->Faces[0].MipLevels[0]->GetWidth();
		if (height) *height = tex->Faces[0].MipLevels[0]->GetHeight();
		if (mipCount) *mipCount = tex->Faces[0].MipLevels.Length();

		glBindTexture(GL_TEXTURE_2D, handle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		if (tex->Faces[0].MipLevels.Length() > 1)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		for (int i = 0; i < tex->Faces[0].MipLevels.Length(); i++)
		{
			Image* mip = tex->Faces[0].MipLevels[i];

#if defined(XLI_GLES2_ETC_SUPPORT)

			if (tex->Faces[0].MipLevels[i]->GetFormat() == FormatCompressedRGB_ETC1)
			{
				glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_ETC1_RGB8_OES, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());  
				continue;
			}
#endif
#if defined(XLI_GLES2_PVRTC_SUPPORT)
            
			if (tex->Faces[0].MipLevels[i]->GetFormat() == FormatCompressedRGB_PVRTC_4BPP)
			{                
				glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
				continue;
			}
			if (tex->Faces[0].MipLevels[i]->GetFormat() == FormatCompressedRGB_PVRTC_2BPP)
			{
				glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
				continue;
			}
			if (tex->Faces[0].MipLevels[i]->GetFormat() == FormatCompressedRGBA_PVRTC_4BPP)
			{
				glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
				continue;
			}
			if (tex->Faces[0].MipLevels[i]->GetFormat() == FormatCompressedRGBA_PVRTC_2BPP)
			{
				glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
				continue;
			}
#endif

			Managed<Xli::Bitmap> bmp = mip->ToBitmap();

			GLenum glFormat;
			GLenum glType;

			XliToGLFormat(bmp->GetFormat(), glFormat, glType);

			glTexImage2D(GL_TEXTURE_2D, i, glFormat, bmp->GetWidth(), bmp->GetHeight(), 0, glFormat, glType, bmp->GetData());
		}
        
        GLCheckError();
	}

	void GLTexture2D::SetData(GLuint handle, Bitmap* bitmap, bool mipmaps, int* width, int* height, int* mips)
	{
		Managed<Bitmap> bmp = bitmap->Pack();
		SetData(handle, bmp->GetWidth(), bmp->GetHeight(), bmp->GetFormat(), bmp->GetData(), mipmaps, mips);
		if (width) *width = bmp->GetWidth();
		if (height) *height = bmp->GetHeight();
	}

	void GLTexture2D::SetData(int width, int height, Format format, void* data, bool mipmaps)
	{
		this->w = width;
		this->h = height;
		SetData(handle, width, height, format, data, mipmaps, &this->mips);
	}

	void GLTexture2D::SetData(Bitmap* bitmap, bool mipmaps)
	{
		SetData(handle, bitmap, mipmaps, &this->w, &this->h, &this->mips);
	}

	void GLTexture2D::SetData(Texture* tex)
	{
		SetData(handle, tex, &this->w, &this->h, &this->mips);
	}

	void GLTexture2D::FilterLinear() const
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLCheckError();
	}
	void GLTexture2D::FilterBilinear() const
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLCheckError();
	}
	void GLTexture2D::FilterTrilinear() const
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLCheckError();
	}
	void GLTexture2D::FilterNearest() const
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GLCheckError();
	}

	void GLTexture2D::WrapClamp() const
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		GLCheckError();
	}
	void GLTexture2D::WrapRepeat() const
	{
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		GLCheckError();
	}
}
