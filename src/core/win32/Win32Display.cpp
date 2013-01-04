#include <Xli/Display.h>
#include <Xli/Win32Header.h>
#include <Xli/Array.h>

namespace Xli
{
	static Array<HMONITOR>* monitors = 0;
	static int initCount = 0;

	BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
	{
		((Array<HMONITOR>*)dwData)->Add(hMonitor);
		return TRUE;
	}

	void Display::Init()
	{
		if (!initCount)
		{
			monitors = new Array<HMONITOR>();
			EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)monitors);
		}

		initCount++;
	}

	void Display::Shutdown()
	{
		initCount--;

		if (initCount == 0)
		{
			monitors->Release();
			monitors = 0;
		}
		else if (initCount < 0)
		{
			XLI_THROW_BAD_DELETE;
		}
	}

	static void AssertInit()
	{
		if (!initCount)
		{
			Display::Init();
			atexit(Display::Shutdown);
		}
	}

	int Display::GetCount()
	{
		AssertInit();
		return monitors->Length();
	}

	Recti Display::GetRect(int index)
	{
		AssertInit();
		HMONITOR hMonitor = monitors->Get(index);

		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMonitor, &info);

		return Recti(info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom);
	}
}
