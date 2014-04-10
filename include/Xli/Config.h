#ifndef __XLI_CONFIG_H__
#define __XLI_CONFIG_H__

#if defined(WIN32)
# define XLI_PLATFORM_WIN32
# if defined(_MSC_VER) && (_MSC_VER >= 1400)
#  define XLI_COMPILER_MSVC
# endif

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

#ifndef XLI_DEBUG
# ifndef NDEBUG
#   define XLI_DEBUG
# endif
#endif

#ifdef XLI_DEBUG
# define XLI_RANGE_CHECK 1
# define XLI_NULL_POINTER_CHECK 1
#endif

#define XLI_INLINE inline
#define XLI_NOEXCEPT throw()

#define XLI_FILE __FILE__
#define XLI_LINE __LINE__

#ifdef XLI_COMPILER_MSVC // Visual C++ specific
# define XLI_FUNCTION __FUNCTION__ //__FUNCSIG__
#else
# define sscanf_s(str, ...) sscanf(str, __VA_ARGS__)
# define sprintf_s(buf, bufSize, format, ...) sprintf(buf, format, __VA_ARGS__)
# define vsnprintf_s(buf, bufSize, maxCount, format, argList) vsprintf(buf, format, argList)
# define XLI_FUNCTION __PRETTY_FUNCTION__
#endif



#endif
