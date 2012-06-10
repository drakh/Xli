#include <XliGL/GLFramebuffer.h>


#ifndef XLI_GLES2
# define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
/*
# define glGenFramebuffers glGenFramebuffersEXT
# define glBindFramebuffer glBindFramebufferEXT
# define glFramebufferTexture2D glFramebufferTexture2DEXT
# define glGenRenderBuffers glGenRenderbuffersEXT
# define glFramebufferRenderBuffer glFramebufferRenderbufferEXT
# define glBindRenderbuffer glBindRenderbufferEXT
# define glRenderbufferStorage glRenderbufferStorageEXT
# define glBindRenderbuffer glBindRenderbufferEXT
# define glDeleteFramebuffers glDeleteFramebuffersEXT
# define glDeleteRenderbuffers glDeleteRenderbuffersEXT
# define GL_FRAMEBUFFER GL_FRAMEBUFFER_EXT
# define GL_DEPTH_ATTACHMENT GL_DEPTH_ATTACHMENT_EXT
# define GL_RENDERBUFFER GL_RENDERBUFFER_EXT
# define GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_EXT
# define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
# define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT
# define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
# define GL_FRAMEBUFFER_INCOMPLETE_FORMATS GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
# define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT
# define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT
# define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB
# define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT
*/
#endif

namespace Xli
{
	void GLFramebuffer::init(const List<GLTexture2D*>& colorBuffers, bool createDepthBuffer)
	{
		// create a framebuffer of the two
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// attach color buffers
		for (int i=0; i<colorBuffers.Length(); i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,  colorBuffers[i]->GetHandle(), 0);
			bufs.Add(colorBuffers[i]);
			//colorBuffers[i]->AddRef();
		}

#ifndef XLI_GLES2
		static const GLenum s = GL_COLOR_ATTACHMENT0;
		GLenum bufs[16] = { s+0,s+1,s+2,s+3,s+4,s+5,s+6,s+7,s+8,s+9,s+10,s+11,s+12,s+13,s+14,s+15 };
		glDrawBuffers(colorBuffers.Length(), bufs);
#else
		if (colorBuffers.Length() > 1) XLI_THROW("MRT not supported in GLES2");
#endif

		depthBuffer = -1;

		if (createDepthBuffer)
		{
			// create a depth renderbuffer
			glGenRenderbuffers(1, &depthBuffer);
			UpdateDepthBufferSize();

			// attach depthbuffer (if any)
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		}


		int code = glCheckFramebufferStatus(GL_FRAMEBUFFER); 

		if (code != GL_FRAMEBUFFER_COMPLETE) 
		{
			String str;	
			
			switch(code)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: str = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS: str = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: str = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
				case GL_FRAMEBUFFER_UNSUPPORTED: str = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
#ifndef XLI_GLES2
				//case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:	str = "GL_FRAMEBUFFER_INCOMPLETE_FORMATS"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: str = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:	str = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB: str = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT"; break;
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: str = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
#endif
			}

			XLI_THROW((String)"Framebuffer incomplete: " + str);
		}
	}

	GLFramebuffer::GLFramebuffer(GLTexture2D* colorBuffer, bool createDepthBuffer)
	{
		List<GLTexture2D*> lst;
		lst.Add(colorBuffer);
		init(lst, createDepthBuffer);
	}

	void GLFramebuffer::UpdateDepthBufferSize()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
#ifndef XLI_GLES2
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, bufs[0]->GetWidth(), bufs[0]->GetHeight());
#else
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, bufs[0]->GetWidth(), bufs[0]->GetHeight());
#endif
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		GLCheckError();
	}

	GLFramebuffer::GLFramebuffer(const List<GLTexture2D*>& colorBuffers, bool createDepthBuffer)
	{
		init(colorBuffers, createDepthBuffer);
	}

	GLFramebuffer::~GLFramebuffer()
	{
		glDeleteFramebuffers(1, &fbo);
		if (depthBuffer != -1) glDeleteRenderbuffers(1, &depthBuffer);
	}
}
