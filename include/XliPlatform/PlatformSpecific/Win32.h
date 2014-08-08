#ifndef __XLI_PLATFORM_SPECIFIC_WIN32_H__
#define __XLI_PLATFORM_SPECIFIC_WIN32_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Undefine conflicting names
#undef MessageBox
#undef GetSystemDirectory
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef CreateDirectory
#undef ChangeDirectory
#undef DeleteFile
#undef MoveFile
#undef CreateMutex
#undef GetCurrentThread

#include <Xli/String.h>

namespace Xli
{
    class Window;

    namespace PlatformSpecific
    {
        /**
            \ingroup XliPlatform
        */
        class Win32
        {
        public:
            /**
                Returns a String describing the error code returned from ::GetLastError()
            */
            static String GetLastErrorString();

            static void RegisterTouchWindow(Window* wnd);
            static HWND GetWindowHandle(Window* wnd);
            static void SetWindowIconByID(Window* wnd, int id);
        };
    }
}

#endif
