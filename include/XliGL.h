#ifndef __XLI_GL_H__
#define __XLI_GL_H__

#include <Xli/Config.h>

#if defined(XLI_PLATFORM_WINDOWS)
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <XliGL/glew.h>
# include <GL/gl.h>

#elif defined(XLI_PLATFORM_ANDROID)
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
# define XLI_GL_ES2
# define XLI_GL_ETC_SUPPORTED

#elif defined(XLI_PLATFORM_IOS)
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
# define XLI_GL_ES2
# define XLI_GL_PVRTC_SUPPORTED

#elif defined(XLI_PLATFORM_MACOSX)
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <XliGL/glew.h>
# include <OpenGL/gl.h>

#elif defined(XLI_PLATFORM_LINUX)
# include <GL/glew.h>
# include <GL/gl.h>

#endif


#endif
