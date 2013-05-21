#include <XliGLHelper.h>
#include <XliMedia/Texture.h>
#include <XliMedia/Bitmap.h>
#include <XliMedia/Ktx.h>
#include <XliMedia/Jpeg.h>
#include <XliMedia/Png.h>
#include <XliMedia/GZip.h>
#include <Xli/Path.h>

namespace Xli
{
	String GLGetErrorString(GLint err)
	{
		switch (err)
		{
		case GL_NO_ERROR: return "No error";
		case GL_INVALID_ENUM: return "An unacceptable value is specified for an enumerated argument";
		case GL_INVALID_VALUE: return "A numeric argument is out of range";
		case GL_INVALID_OPERATION: return "The specified operation is not allowed in the current state";
		case GL_OUT_OF_MEMORY: return "There is not enough memory left to execute the command";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid framebuffer operation";
#ifndef XLI_GL_ES2
		case GL_STACK_OVERFLOW: return "This command would cause a stack overflow";
		case GL_STACK_UNDERFLOW: return " This command would cause a stack underflow";
		case GL_TABLE_TOO_LARGE: return "The specified table exceeds the implementation's maximum supported table size";
#endif
		default: return "Unknown error";
		}
	}

	GLuint GLCompileShader(GLenum shaderType, const String& source)
	{
		GLuint shaderHandle = glCreateShader(shaderType);

#ifdef XLI_GL_ES2

		const char* code[] =
		{
			"precision mediump float;\n",
			source.Data(),
		};

		GLint len[] =
		{
			strlen(code[0]),
			source.Length(),
		};

		glShaderSource(shaderHandle, 2, code, len);

#else

		const char* code[] =
		{
			source.Data(),
		};

		GLint len[] =
		{
			source.Length(),
		};

		glShaderSource(shaderHandle, 1, code, len);

#endif

		glCompileShader(shaderHandle);

		GLint status;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			int len = 0;
			const int bufSize = 4096;
			char buf[bufSize];
			glGetShaderInfoLog(shaderHandle, bufSize, &len, buf);
			XLI_THROW("Error compiling GLSL shader:\n\n" + Xli::String(buf, len));
		}

		XLI_GL_CHECK_ERROR;
		return shaderHandle;
	}

	GLuint GLLinkProgram(GLuint vsHandle, GLuint fsHandle)
	{
		GLuint progHandle = glCreateProgram();

		glAttachShader(progHandle, vsHandle);
		glAttachShader(progHandle, fsHandle);
		glLinkProgram(progHandle);

		GLint status;
		glGetProgramiv(progHandle, GL_LINK_STATUS, &status);

		if (!status)
		{
			int len = 0;
			const int bufSize = 4096;
			char buf[bufSize];
			glGetProgramInfoLog(progHandle, bufSize, &len, buf);
			XLI_THROW("Error linking GLSL program:\n\n" + Xli::String(buf, len));
		}

		glUseProgram(progHandle);

		XLI_GL_CHECK_ERROR;
		return progHandle;
	}

	Texture* GLLoadTextureData(const String& filename, Stream* stream)
	{
		String fnUpper = filename.ToUpper();

		if (fnUpper.EndsWith(".GZ"))
		{
			Managed<Xli::Stream> gz = GZip::CreateReader(stream);
			return GLLoadTextureData(filename.Substring(0, filename.Length() - 3), gz);
		}

		if (fnUpper.EndsWith(".KTX"))
		{
			return Ktx::Load(stream);
		}

		Managed<ImageReader> ir;

		if (fnUpper.EndsWith(".PNG"))
			ir = Png::CreateReader(stream);
		else if (fnUpper.EndsWith(".JPG") || fnUpper.EndsWith(".JPEG"))
			ir = Jpeg::CreateReader(stream);
		else
			XLI_THROW("Unsupported texture extension: " + Path::GetExtension(filename));

		Managed<Bitmap> bmp = ir->ReadBitmap();
		return Texture::Create(bmp);
	}

	static bool TryGetGLFormat(Format format, GLenum& glFormat, GLenum& glType)
	{
		switch (format)
		{
		case FormatRGBA_8_8_8_8_UInt_Normalize:
			glFormat = GL_RGBA;
			glType = GL_UNSIGNED_BYTE;
			return true;

		case FormatRGB_8_8_8_UInt_Normalize:
			glFormat = GL_RGB;
			glType = GL_UNSIGNED_BYTE;
			return true;

		case FormatR_8_UInt_Normalize:
		case FormatL_8_UInt_Normalize:
			glFormat = GL_LUMINANCE;
			glType = GL_UNSIGNED_BYTE;
			return true;

		case FormatRG_8_8_UInt_Normalize:
		case FormatLA_8_8_UInt_Normalize:
			glFormat = GL_LUMINANCE_ALPHA;
			glType = GL_UNSIGNED_BYTE;
			return true;

		default:
			return false;
		}
	}

	GLuint GLCreateTexture(Texture* texData, bool generateMips, GLTextureInfo* outInfo)
	{
		GLuint texHandle;
		glGenTextures(1, &texHandle);

		int width = texData->Faces[0].MipLevels[0]->GetWidth();
		int height = texData->Faces[0].MipLevels[0]->GetHeight();
		int mipCount = texData->Faces[0].MipLevels.Length();
		int depth = 1;

		GLenum glTarget =
			texData->Type == TextureTypeCube ?
				GL_TEXTURE_CUBE_MAP :
				GL_TEXTURE_2D;

		glBindTexture(glTarget, texHandle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		bool compressed = false;

		for (int i = 0; i < texData->Faces.Length(); i++)
		{
			GLenum glFace =
				glTarget == GL_TEXTURE_CUBE_MAP ?
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i :
					GL_TEXTURE_2D;

			for (int j = 0; j < texData->Faces[i].MipLevels.Length(); j++)
			{
				Image* mip = texData->Faces[i].MipLevels[j];

				switch (mip->GetFormat())
				{
#ifdef XLI_GL_ETC_SUPPORTED

				case FormatCompressedRGB_ETC1:
					glCompressedTexImage2D(glFace, j, GL_ETC1_RGB8_OES, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

#endif
#ifdef XLI_GL_PVRTC_SUPPORTED

				case FormatCompressedRGB_PVRTC_4BPP:
					glCompressedTexImage2D(glFace, j, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

				case FormatCompressedRGB_PVRTC_2BPP:
					glCompressedTexImage2D(glFace, j, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

				case FormatCompressedRGBA_PVRTC_4BPP:
					glCompressedTexImage2D(glFace, j, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

				case FormatCompressedRGBA_PVRTC_2BPP:
					glCompressedTexImage2D(glFace, j, GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

#endif

				default:
					{
						Managed<Bitmap> bmp = mip->ToBitmap();

						GLenum glFormat, glType;
						if (!TryGetGLFormat(bmp->GetFormat(), glFormat, glType))
							XLI_THROW("Unsupported texture format: " + FormatInfo::ToString(bmp->GetFormat()));

						glTexImage2D(glFace, j, glFormat, bmp->GetWidth(), bmp->GetHeight(), 0, glFormat, glType, bmp->GetData());
					}

					break;

				// Silence 'switch has default but no cases' warning
				case FormatUnspecified:
					break;
				}
			}
		}

		if (generateMips && !compressed)
		{
			glGenerateMipmap(glTarget);
			GLenum err = glGetError();

			if (err == GL_NO_ERROR)
			{
				int w = width, h = height;

				while (w > 1 || h > 1)
				{
					w /= 2;
					h /= 2;
					mipCount++;
				}
			}
		}

		if (outInfo) 
		{
			outInfo->Target = glTarget;
			outInfo->Width = width;
			outInfo->Height = height;
			outInfo->Depth = depth;
			outInfo->MipCount = mipCount;
		}

		XLI_GL_CHECK_ERROR;
		return texHandle;
	}
}
