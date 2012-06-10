#include <XliGL/GLFontRenderer.h>
#include <XliGL/GLHelpers.h>
#include <XliGraphics/Semantic.h>

namespace Xli
{
	GLFontRenderer::GLFontRenderer(Vector2i resolution, int maxChars)
	{
		this->Resolution = (Vector2)resolution;
		this->maxChars = maxChars;

		vertices.Reserve(maxChars * 4);

		Array<UInt16> indices;
		indices.Reserve(maxChars * 6);

		for (int i = 0; i < maxChars; i++)
		{
			indices.Add((UInt16)(i*4 + 0));
			indices.Add((UInt16)(i*4 + 1));
			indices.Add((UInt16)(i*4 + 2));
			indices.Add((UInt16)(i*4 + 0));
			indices.Add((UInt16)(i*4 + 2));
			indices.Add((UInt16)(i*4 + 3));
		}

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Length() * sizeof(UInt16), indices.Data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, maxChars * 4 * sizeof(GLFont::Vertex), 0, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

		prog = GLHelpers::LoadProgram("data/text");
		attrPos = prog->GetAttribLocation(Semantic::Position);
		attrTC = prog->GetAttribLocation(Semantic::TexCoord);
	}

	void GLFontRenderer::Begin(GLFont* font)
	{
		this->font = font;
		vertices.Clear();
	}

	Vector2i GLFontRenderer::WriteString(const String& str, Vector2i cursor, const Recti& rect)
	{
		return font->WriteString(str, cursor, rect, vertices);
	}

	Vector2i GLFontRenderer::WriteString(const String& str, Vector2i cursor)
	{
		return font->WriteString(str, cursor, Recti(0, 0, (int)Resolution.X, (int)Resolution.Y), vertices);
	}

	void GLFontRenderer::WriteString(const String& str, const Recti& rect, int textAlign)
	{
		font->WriteString(str, rect, textAlign, vertices);
	}

	void GLFontRenderer::End(const Vector4& color)
	{
		if (vertices.Length() > 0)
		{
			prog->Use();
			prog->SetVector2("Resolution", Resolution);
			prog->SetVector4("Color", color);
			prog->SetSampler2D("Tex", 0, font->Texture->GetHandle());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLFont::Vertex) * Min(maxChars * 4, vertices.Length()), vertices.Data());

			glEnableVertexAttribArray(attrPos);
			glEnableVertexAttribArray(attrTC);

			glVertexAttribPointer(attrPos, 2, GL_SHORT, GL_FALSE, sizeof(GLFont::Vertex), (const GLvoid*)0); //offsetof(GLFont::Vertex, Position));
			glVertexAttribPointer(attrTC, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(GLFont::Vertex), (const GLvoid*)4); //offsetof(GLFont::Vertex, TexCoord));

			glDrawElements(GL_TRIANGLES, Min(maxChars, vertices.Length() / 4) * 6, GL_UNSIGNED_SHORT, 0);

			glDisableVertexAttribArray(attrPos);
			glDisableVertexAttribArray(attrTC);

			glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
		}

		font = 0;
	}

	Vector2i GLFontRenderer::DrawString(GLFont* font, const String& str, const Vector4& color, Vector2i cursor, const Recti& rect)
	{
		Begin(font);
		cursor = WriteString(str, cursor, rect);
		End(color);
		return cursor;
	}

	Vector2i GLFontRenderer::DrawString(GLFont* font, const String& str, const Vector4& color, Vector2i cursor)
	{
		Begin(font);
		cursor = WriteString(str, cursor);
		End(color);
		return cursor;
	}

	void GLFontRenderer::DrawString(GLFont* font, const String& str, const Vector4& color, const Recti& rect, int textAlign)
	{
		Begin(font);
		WriteString(str, rect, textAlign);
		End(color);
	}
}
