#ifndef __XLI_GL_HEADERS_H__
#define __XLI_GL_HEADERS_H__

#include <Xli/Config.h>

#if defined(XLI_PLATFORM_WINDOWS)
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <XliGL/GLEW/glew.h>
# include <GL/gl.h>

#elif defined(XLI_PLATFORM_ANDROID)
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
# define XLI_GLES2
# define XLI_GLES2_ETC_SUPPORT

#elif defined(XLI_PLATFORM_IOS)
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
# define XLI_GLES2
# define XLI_GLES2_PVRTC_SUPPORT

#elif defined(XLI_PLATFORM_MACOSX)
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <XliGL/GLEW/glew.h>
# include <OpenGL/gl.h>

#elif defined(XLI_PLATFORM_LINUX)
# include <GL/glew.h>
# include <GL/gl.h>

#endif


#endif
