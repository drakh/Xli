#include <Xli/Win32Helpers.h>
#include <Xli/Win32Header.h>

namespace Xli
{
	String Win32Helpers::GetLastErrorString()
	{
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError(); 

		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		String msg = (Utf16Char*)lpMsgBuf;
		LocalFree(lpMsgBuf);

		return msg;
	}
}
