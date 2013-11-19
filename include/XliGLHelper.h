#ifndef __XLI_GL_HELPER_H__
#define __XLI_GL_HELPER_H__

/**
	\defgroup XliGLHelper 
	\brief OpenGL helper functions
*/

#include <XliGL.h>
#include <Xli/Vector3.h>

namespace Xli
{
	/**
		\addtogroup XliGLHelper
		@{
	 */

	String GLGetErrorString(GLint err);
	void GLCheckErrorImpl(const char* func, int line);

#define XLI_GL_CHECK_ERROR ::Xli::GLCheckErrorImpl(XLI_FUNCTION, XLI_LINE)

	GLuint GLCompileShader(GLenum shaderType, const String& source);
	GLuint GLLinkProgram(GLuint vsHandle, GLuint fsHandle);

	class Texture;
	Texture* GLLoadTextureData(const String& filename, Stream* stream);

	struct GLTextureInfo
	{
		GLenum Target;
		int Width;
		int Height;
		int Depth;
		int MipCount;
	};

	GLuint GLCreateTexture(Texture* texData, bool generateMips = true, GLTextureInfo* outInfo = 0);

    class Matrix4;
	Matrix4 GLLookAtMatrix(const Vector3& eye, const Vector3& center, const Vector3& upVec);
	Matrix4 GLPerspectiveMatrix(float fovRadians, float aspect, float zNear, float zFar);
	Matrix4 GLOrthoMatrix(float left, float right, float bottom, float top, float nearval = -1.0f, float farval = 1.0f);

	/** @}*/
}


#endif
