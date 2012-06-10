#ifndef __XLI_GL_FONT_RENDERER_H__
#define __XLI_GL_FONT_RENDERER_H__

#include "GLFont.h"
#include "GLProgram.h"

namespace Xli
{
	class GLFontRenderer: public Object
	{
		Array<GLFont::Vertex> vertices;

		Managed<GLProgram> prog;
		GLint attrPos, attrTC;
		GLuint ibo, vbo;
		int maxChars;

		Shared<GLFont> font;

	public:
		Vector2 Resolution;

		GLFontRenderer(Vector2i resolution, int maxChars = 1024);
		virtual ~GLFontRenderer() {}

		void Begin(GLFont* font);
		Vector2i WriteString(const String& str, Vector2i cursor, const Recti& rect);
		Vector2i WriteString(const String& str, Vector2i cursor);
		void WriteString(const String& str, const Recti& rect, int textAlign);
		void End(const Vector4& color);

		Vector2i DrawString(GLFont* font, const String& str, const Vector4& color, Vector2i cursor, const Recti& rect);
		Vector2i DrawString(GLFont* font, const String& str, const Vector4& color, Vector2i cursor);
		void DrawString(GLFont* font, const String& str, const Vector4& color, const Recti& rect, int textAlign);
	};
}

#endif
