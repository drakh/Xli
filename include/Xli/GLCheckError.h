#ifndef __XLI_GL_CHECK_ERROR_H__
#define __XLI_GL_CHECK_ERROR_H__

#include <Xli/GLHeaders.h>
#include <Xli/Console.h>

namespace Xli
{
	/**
		\addtogroup GL
		@{
	 */

	String GLGetErrorString(GLint err);

	static inline void GLCheckErrorImpl(const char* func, int line)
	{
		GLint err = glGetError();
		if (err == GL_NO_ERROR) return;
		Xli::ErrorPrintLine("GL ERROR: " + Xli::String::HexFromInt(err) + " - " + GLGetErrorString(err) + " (in " + func + ":" + line + ")");
	}

#define XLI_GL_CHECK_ERROR() ::Xli::GLCheckErrorImpl(XLI_FUNC, __LINE__)

	/** @}*/
}


#endif
