#ifndef __XLI_CONFIG_H__
#define __XLI_CONFIG_H__

#if defined(WIN32)
# define XLI_PLATFORM_WIN32

#elif defined(ANDROID)
# define XLI_PLATFORM_ANDROID

#elif defined(__APPLE__)
# include <TargetConditionals.h>
# if TARGET_OS_IPHONE
#   define XLI_PLATFORM_IOS
# else
#   define XLI_PLATFORM_OSX
# endif

#elif defined(__linux)
# define XLI_PLATFORM_LINUX

#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
# define XLI_COMPILER_MSVC
#endif

#ifndef XLI_DEBUG
# ifdef DEBUG
#   define XLI_DEBUG
# elif _DEBUG
#   define XLI_DEBUG
# endif
#endif

#ifndef XLI_DEBUG
# ifndef NDEBUG
#   define XLI_DEBUG
# endif
#endif

#ifdef XLI_DEBUG
# define XLI_RANGE_CHECK 1
# define XLI_NULL_POINTER_CHECK 1
# define XLI_DEBUG_PRINT(x) ::Xli::PrintLine(x)
#endif

#ifdef XLI_COMPILER_MSVC // Visual C++ specific
# define XLI_FUNCTION __FUNCTION__ //__FUNCSIG__
# ifdef XLI_DEBUG
#   define XLI_DEBUG_BREAK __debugbreak()
# endif
#else
# define sscanf_s(str, ...) sscanf(str, __VA_ARGS__)
# define sprintf_s(buf, bufSize, format, ...) sprintf(buf, format, __VA_ARGS__)
# define vsnprintf_s(buf, bufSize, maxCount, format, argList) vsprintf(buf, format, argList)
# if 1 // GCC specific (TODO: Add GCC check)
#    define XLI_FUNCTION __PRETTY_FUNCTION__
#    ifdef XLI_DEBUG
#       if !defined(XLI_PLATFORM_IOS) && !defined(XLI_PLATFORM_ANDROID)
#           define XLI_DEBUG_BREAK __builtin_trap()
#       endif
#    endif
# else
#    define XLI_FUNCTION __FUNCTION__
# endif
#endif

#define XLI_LINE __LINE__

#ifndef XLI_DEBUG_BREAK
# define XLI_DEBUG_BREAK
#endif

#ifndef XLI_DEBUG_PRINT
# define XLI_DEBUG_PRINT(x)
#endif

#define XLI_INLINE inline


#endif
