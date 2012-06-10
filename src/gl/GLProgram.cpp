#include <XliGL/GLProgram.h>

namespace Xli
{
	GLuint GLProgram::CompileShader(GLenum shaderType, const CharString& fn, const CharString& src)
	{
		GLuint handle = glCreateShader(shaderType);

		if (!handle)
		{
			XLI_THROW(String::Format("Error compiling '%s':\n\n%s", fn.Data(), "GL handle was NULL"));
		}
        
#ifdef XLI_GLES2

		const char* code[] = 
		{
			"precision mediump float;\n",
			src.Data(),
		};

		GLint len[] =
		{
			strlen(code[0]),
			src.Length(),
		};

		glShaderSource(handle, 2, code, len);

#else

		const char* code[] = 
		{
			src.Data(),
		};

		GLint len[] =
		{
			src.Length(),
		};

		glShaderSource(handle, 1, code, len);

#endif

		glCompileShader(handle);

		GLint status;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
		
		if (status != GL_TRUE)
		{
			char buf[4096];
            buf[0] = 0;
            
			glGetShaderInfoLog(handle, 4096, 0, buf);
            
			XLI_THROW(String::Format("Error compiling '%s':\n\n%s", fn.Data(), buf));
		}

		return handle;
	}

	GLuint GLProgram::LinkProgram(GLuint vs, GLuint ps)
	{
		GLuint prog = glCreateProgram();
        
		if (!prog)
		{
			XLI_THROW("Error linkning shader program: GL handle was NULL");
		}
        
		glAttachShader(prog, vs);
		glAttachShader(prog, ps);
		glLinkProgram(prog);
		
		GLint linked;
		glGetProgramiv(prog, GL_LINK_STATUS, &linked);

		if (linked!=GL_TRUE)
		{
			int infologLength = 0;
			int charsWritten = 0;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infologLength);
			
			char* infoLog = new char[infologLength];
			glGetProgramInfoLog(prog, infologLength, &charsWritten, infoLog);

			String errString = "Error linking shader program: " + (String)infoLog;
			delete [] infoLog;
			
			XLI_THROW(errString);
		}

		glUseProgram(prog);
		GLCheckError();
		glUseProgram(0);

		return prog;
	}

	GLProgram::GLProgram(const String& vertFn, const CharString& vertSrc, const String& fragFn, const CharString& fragSrc)
	{
		vs = CompileShader(GL_VERTEX_SHADER, vertFn, vertSrc);
		fs = CompileShader(GL_FRAGMENT_SHADER, fragFn, fragSrc);
		prog = LinkProgram(vs, fs);
	}

	GLProgram::~GLProgram()
	{
		glUseProgram(0);
		glDetachShader(prog, vs);
		glDetachShader(prog, fs);
		glDeleteProgram(prog);
		glDeleteShader(vs);
		glDeleteShader(fs);
		GLCheckError();
	}
}
