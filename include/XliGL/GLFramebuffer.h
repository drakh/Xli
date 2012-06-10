#ifndef __XLI_GL_FRAMEBUFFER_H__
#define __XLI_GL_FRAMEBUFFER_H__

#include "GLTexture2D.h"
#include <Xli/List.h>

namespace Xli
{
	class GLFramebuffer: public Xli::Object
	{
		GLuint fbo;
		GLuint depthBuffer;
		Array<Shared<GLTexture2D> > bufs;

		void init(const List<GLTexture2D*>& colorBuffers, bool createDepthBuffer);

	public:
		GLFramebuffer(GLTexture2D* colorBuffers, bool createDepthBuffer = false);
		GLFramebuffer(const List<GLTexture2D*>& colorBuffers, bool createDepthBuffer = false);
		~GLFramebuffer();

		void UpdateDepthBufferSize();

		GLuint GetHandle() const { return fbo; }
	};
}


#endif
