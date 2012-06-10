#ifndef __XLI_GL_HELPERS_H__
#define __XLI_GL_HELPERS_H__

#include "GLTexture2D.h"
#include "GLProgram.h"
#include "GLFont.h"

namespace Xli
{
	class GLHelpers
	{
	public:
		static GLProgram* LoadProgram(const String& vertFn, const String& fragFn);
		static GLProgram* LoadProgram(const String& fnPrefix);
		static GLTexture2D* LoadTexture2D(const String& fn, bool mips);
		static GLFont* LoadFont(const String& fn, int fontSize, const String& charSet = "", bool pow2 = false);

		static void DrawQuad(GLProgram* prog);
	};
}


#endif
