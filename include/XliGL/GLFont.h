#ifndef __XLI_GL_FONT_H__
#define __XLI_GL_FONT_H__

#include <XliGraphics/SpriteFont.h>
#include "GLTexture2D.h"

namespace Xli
{
	class GLFont: public SpriteFont
	{
	public:
		Managed<GLTexture2D> Texture;

		GLFont(BitmapFont* bf);
		virtual ~GLFont() {}
	};
}

#endif
