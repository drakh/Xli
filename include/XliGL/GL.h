#ifndef __XLI_GL_GL_H__
#define __XLI_GL_GL_H__

#include <Xli/Config.h>

#if defined(XLI_PLATFORM_WIN32)
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <XliGL/3rdparty/glew.h>
# include <GL/gl.h>
# define XLI_GL_DESKTOP

#elif defined(XLI_PLATFORM_ANDROID)
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
# define XLI_GL_ES2

#elif defined(XLI_PLATFORM_IOS)
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
# define XLI_GL_ES2

#elif defined(XLI_PLATFORM_OSX)
# ifndef GLEW_STATIC
#   define GLEW_STATIC
# endif
# include <XliGL/3rdparty/glew.h>
# include <OpenGL/gl.h>
# define XLI_GL_DESKTOP

#elif defined(XLI_PLATFORM_LINUX)
# ifdef __arm__
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#  define XLI_GL_ES2
# else
#  include <GL/glew.h>
#  include <GL/gl.h>
#  define XLI_GL_DESKTOP
# endif

#else
#error "Unsupported platform"

#endif


#endif
