#include <XliPlatform/PlatformSpecific/Win32Helpers.h>
#include <XliPlatform/PlatformSpecific/Win32Header.h>
#include <Xli/Unicode.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        String Win32Helpers::GetLastErrorString()
        {
            LPWSTR lpMsgBuf;
            DWORD dw = GetLastError();

            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf, // Cast because callee is allocating buffer
                0, NULL);

            String msg = Unicode::Utf16To8(lpMsgBuf);
            LocalFree(lpMsgBuf);

            return msg;
        }
    }
}
