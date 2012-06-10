#ifndef __XLI_GL_COMMON_H__
#define __XLI_GL_COMMON_H__

#include "GLHeaders.h"

#include <Xli/Exception.h>
#include <Xli/Console.h>

namespace Xli
{
	static inline CharString GLGetErrorString(GLint err)
	{
		switch (err)
		{
		case GL_NO_ERROR: return "No error";
		case GL_INVALID_ENUM: return "An unacceptable value is specified for an enumerated argument";
		case GL_INVALID_VALUE: return "A numeric argument is out of range";
		case GL_INVALID_OPERATION: return "The specified operation is not allowed in the current state";
		case GL_OUT_OF_MEMORY: return "There is not enough memory left to execute the command";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid framebuffer operation";
#ifndef XLI_GLES2
		case GL_STACK_OVERFLOW: return "This command would cause a stack overflow";
		case GL_STACK_UNDERFLOW: return " This command would cause a stack underflow";
		case GL_TABLE_TOO_LARGE: return "The specified table exceeds the implementation's maximum supported table size";
#endif
		default: return "Unknown error: " + CharString::HexFromInt(err);
		}
	}

	static inline void GLCheckErrorImpl(const char* func, int line)
	{
		GLint err = glGetError();
		if (err == GL_NO_ERROR) return;
		XLI_BREAK_THROW("GL ERROR: " + GLGetErrorString(err) + " (in " + func + ":" + line + ")");
	}

#define GLCheckError() GLCheckErrorImpl(XLI_FUNC, __LINE__)
}

#endif
