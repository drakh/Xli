#ifndef __XLI_PLATFORM_SPECIFIC_WIN32_HEADER_H__
#define __XLI_PLATFORM_SPECIFIC_WIN32_HEADER_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef MessageBox
#undef GetSystemDirectory
#undef GetCurrentDirectory
#undef CreateDirectory
#undef GetCurrentDirectory
#undef ChangeDirectory
#undef DeleteFile
#undef MoveFile
#undef CreateMutex

#endif
