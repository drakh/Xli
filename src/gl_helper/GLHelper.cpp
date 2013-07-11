#include <XliGLHelper.h>
#include <XliMedia/Texture.h>
#include <XliMedia/Bitmap.h>
#include <XliMedia/Ktx.h>
#include <XliMedia/Jpeg.h>
#include <XliMedia/Png.h>
#include <XliMedia/GZip.h>
#include <Xli/Path.h>
#include <cstring>

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
#ifdef XLI_GL_DESKTOP
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

		const char* code[] =
		{
#ifdef XLI_GL_ES2
			"precision mediump float;\n",
#else
            "",
#endif
			source.Data(),
		};

		GLint len[] =
		{
			(GLint)strlen(code[0]),
			source.Length(),
		};

		glShaderSource(shaderHandle, 2, code, len);
		glCompileShader(shaderHandle);

		GLint compileStatus;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileStatus);

		if (!compileStatus)
		{
			int len = 0;
			const int bufSize = 4096;
			char buf[bufSize];
			glGetShaderInfoLog(shaderHandle, bufSize, &len, buf);
			XLI_THROW("Error compiling GLSL shader:\n\n" + String(buf, len));
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

		GLint linkStatus;
		glGetProgramiv(progHandle, GL_LINK_STATUS, &linkStatus);

		if (!linkStatus)
		{
			int len = 0;
			const int bufSize = 4096;
			char buf[bufSize];
			glGetProgramInfoLog(progHandle, bufSize, &len, buf);
			XLI_THROW("Error linking GLSL program:\n\n" + String(buf, len));
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
			Managed<Stream> gz = GZip::CreateReader(stream);
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
			XLI_THROW("Unsupported texture extension '" + Path::GetExtension(filename) + "'");

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

		GLenum texTarget =
			texData->Type == TextureTypeCube ?
				GL_TEXTURE_CUBE_MAP :
				GL_TEXTURE_2D;

		glBindTexture(texTarget, texHandle);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		bool compressed = false;

		for (int i = 0; i < texData->Faces.Length(); i++)
		{
			GLenum texFace =
				texTarget == GL_TEXTURE_CUBE_MAP ?
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i :
					GL_TEXTURE_2D;

			for (int j = 0; j < texData->Faces[i].MipLevels.Length(); j++)
			{
				Image* mip = texData->Faces[i].MipLevels[j];

				switch (mip->GetFormat())
				{
#ifdef GL_ETC1_RGB8_OES

				case FormatCompressedRGB_ETC1:
					glCompressedTexImage2D(texFace, j, GL_ETC1_RGB8_OES, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

#endif
#ifdef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG

				case FormatCompressedRGB_PVRTC_4BPP:
					glCompressedTexImage2D(texFace, j, GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

#endif
#ifdef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
                        
				case FormatCompressedRGB_PVRTC_2BPP:
					glCompressedTexImage2D(texFace, j, GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

#endif
#ifdef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
                        
				case FormatCompressedRGBA_PVRTC_4BPP:
					glCompressedTexImage2D(texFace, j, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;
                        
#endif
#ifdef GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG

				case FormatCompressedRGBA_PVRTC_2BPP:
					glCompressedTexImage2D(texFace, j, GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, mip->GetWidth(), mip->GetHeight(), 0, mip->GetSizeInBytes(), mip->GetData());
					compressed = true;
					break;

#endif

				default:
					{
						Managed<Bitmap> bmp = mip->ToBitmap();

						GLenum glFormat, glType;
						if (!TryGetGLFormat(bmp->GetFormat(), glFormat, glType))
							XLI_THROW("Unsupported texture format: " + FormatInfo::ToString(bmp->GetFormat()));

						glTexImage2D(texFace, j, glFormat, bmp->GetWidth(), bmp->GetHeight(), 0, glFormat, glType, bmp->GetData());
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
			glGenerateMipmap(texTarget);
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
			outInfo->Target = texTarget;
			outInfo->Width = width;
			outInfo->Height = height;
			outInfo->Depth = depth;
			outInfo->MipCount = mipCount;
		}

		XLI_GL_CHECK_ERROR;
		return texHandle;
	}

	Matrix4 GLLookAtMatrix(const Vector3& eye, const Vector3& center, const Vector3& upVec)
	{
		Vector3 forward = Normalize(center - eye);

		/* Side = forward x up */
		Vector3 side = Normalize(Cross(forward, upVec));

		/* Recompute up as: up = side x forward */
		Vector3 up = Cross(side, forward);

		Matrix4 m = Matrix4::Identity();
			
		m[ 0] = side[0];
		m[ 4] = side[1];
		m[ 8] = side[2];

		m[ 1] = up[0];
		m[ 5] = up[1];
		m[ 9] = up[2];

		m[ 2] = -forward[0];
		m[ 6] = -forward[1];
		m[10] = -forward[2];

		return m * Matrix4::Translation(-eye);
	}

	Matrix4 GLPerspectiveMatrix(float fovRadians, float aspect, float zNear, float zFar)
	{
		fovRadians = (float)0.5 * fovRadians;

		Matrix4 m = Matrix4::Identity();
		float sine, cotangent, deltaZ;

		deltaZ = zFar - zNear;
		sine = Sin(fovRadians/(float)2);

		if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
			return m;

		cotangent = Cos(fovRadians/(float)2) / sine;

		m[ 0] = cotangent / aspect;
		m[ 5] = cotangent;
		m[10] = -(zFar + zNear) / deltaZ;
		m[11] = -1;
		m[14] = -2 * zNear * zFar / deltaZ;
		m[15] = 0;

		return m;
	}

	Matrix4 GLOrthoMatrix(float left, float right, float bottom, float top, float nearval, float farval)
	{
		Matrix4 m;

		#define M(row, col)  m[col*4+row]
		M(0,0) = (float)2 / (right-left);
		M(0,1) = (float)0;
		M(0,2) = (float)0;
		M(0,3) = -(right+left) / (right-left);

		M(1,0) = (float)0;
		M(1,1) = (float)2 / (top-bottom);
		M(1,2) = (float)0;
		M(1,3) = -(top+bottom) / (top-bottom);

		M(2,0) = (float)0;
		M(2,1) = (float)0;
		M(2,2) = (float)-2 / (farval-nearval);
		M(2,3) = -(farval+nearval) / (farval-nearval);

		M(3,0) = (float)0;
		M(3,1) = (float)0;
		M(3,2) = (float)0;
		M(3,3) = (float)1;
		#undef M

		return m;
	}
}
