#ifndef __XLI_GL_PROGRAM_H__
#define __XLI_GL_PROGRAM_H__

#include "GLTexture2D.h"
#include <Xli/Matrix4.h>

namespace Xli
{
	class GLProgram: public Object
	{
		GLuint vs, fs, prog;

	public:
		static GLuint CompileShader(GLenum shaderType, const CharString& fn, const CharString& src);
		static GLuint LinkProgram(GLuint vs, GLuint ps);

		GLProgram(const String& vertFn, const CharString& vertSrc, const String& fragFn, const CharString& fragSrc);
		virtual ~GLProgram();

		GLuint GetHandle() const
		{
			return prog;
		}
		GLint GetAttribLocation(const char* name) const
		{
			return glGetAttribLocation(prog, name);
		}
		GLint GetUniformLocation(const char* name) const
		{
			return glGetUniformLocation(prog, name);
		}
		void Use()  const
		{
			glUseProgram(prog);
			GLCheckError();
		}

		void SetInt(GLint loc, GLuint i) const
		{
			if (loc != -1) glUniform1i(loc, i);
		}
		void SetFloat(GLint loc, GLfloat f) const
		{
			if (loc != -1) glUniform1f(loc, f);
		}
		void SetVector2(GLint loc, const Vector2& v) const
		{
			if (loc != -1) glUniform2f(loc, v.X, v.Y);
		}
		void SetVector3(GLint loc, const Vector3& v) const
		{
			if (loc != -1) glUniform3f(loc, v.X, v.Y, v.Z);
		}
		void SetVector4(GLint loc, const Vector4& v) const
		{
			if (loc != -1) glUniform4f(loc, v.X, v.Y, v.Z, v.W);
		}
		void SetColor(GLint loc, const Color& c) const
		{
			if (loc != -1) glUniform4f(loc, c.R, c.G, c.B, c.A);
		}
		void SetMatrix4(GLint loc, const Matrix4& m) const
		{
		    if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, m.Data());
		}
		void SetSampler2D(GLint loc, int index, GLuint handle) const
		{
			if (loc != -1)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, handle);
				glUniform1i(loc, index);
			}
		}
		void SetSampler2D(GLint loc, int index, GLTexture2D* tex) const
		{
			if (loc != -1)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, tex->GetHandle());
				glUniform1i(loc, index);
			}
		}

		void SetInt(const char* loc, GLint i) const
		{
			SetInt(GetUniformLocation(loc), i);
		}
		void SetFloat(const char* loc, GLfloat f) const
		{
			SetFloat(GetUniformLocation(loc), f);
		}
		void SetVector2(const char* loc, const Vector2& v) const
		{
			SetVector2(GetUniformLocation(loc), v);
		}
		void SetVector3(const char* loc, const Vector3& v) const
		{
			SetVector3(GetUniformLocation(loc), v);
		}
		void SetVector4(const char* loc, const Vector4& v) const
		{
			SetVector4(GetUniformLocation(loc), v);
		}
		void SetMatrix4(const char* loc, const Matrix4& m) const
		{
			SetMatrix4(GetUniformLocation(loc), m);
		}
		void SetSampler2D(const char* loc, int index, GLuint handle) const
		{
			SetSampler2D(GetUniformLocation(loc), index, handle);
		}
		void SetSampler2D(const char* loc, int index, GLTexture2D* tex) const
		{
			SetSampler2D(GetUniformLocation(loc), index, tex);
		}
	};
}


#endif
