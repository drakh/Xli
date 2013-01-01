#ifndef __XLI_GL_CHECK_ERROR_H__
#define __XLI_GL_CHECK_ERROR_H__

#include "GLHeaders.h"

#include <Xli/Exception.h>
#include <Xli/Console.h>

namespace Xli
{
	/**
		\addtogroup GL
		@{
	 */

	CharString GLGetErrorString(GLint err);

	static inline void GLCheckErrorImpl(const char* func, int line)
	{
		GLint err = glGetError();
		if (err == GL_NO_ERROR) return;
		XLI_BREAK_THROW("GL ERROR: " + GLGetErrorString(err) + " (in " + func + ":" + line + ")");
	}

#define GLCheckError() GLCheckErrorImpl(XLI_FUNC, __LINE__)

	/** @}*/
}


#endif
