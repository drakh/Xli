#include <Xli/Display.h>
#include <Xli/PlatformSpecific/Win32Header.h>
#include <Xli/Array.h>
#include <cstdlib>

namespace Xli
{
	static Array<HMONITOR>* monitors = 0;
	static int initCount = 0;

	static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
	{
		((Array<HMONITOR>*)dwData)->Add(hMonitor);
		return TRUE;
	}

	void DisplayInit()
	{
		if (!initCount)
		{
			monitors = new Array<HMONITOR>();
			EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)monitors);
		}

		initCount++;
	}

	void DisplayDone()
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
			DisplayInit();
			atexit(DisplayDone);
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

	bool GetCurrentSettings(int index, DisplaySettings& settings)
	{
		AssertInit();
		HMONITOR hMonitor = monitors->Get(index);

		MONITORINFOEX info;
		info.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &info);

		DEVMODE mode;
		mode.dmSize = sizeof(DEVMODE);
		mode.dmDriverExtra = 0;

		if (EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &mode))
		{
			settings.Resolution.X = mode.dmPelsWidth;
			settings.Resolution.Y = mode.dmPelsHeight;
			settings.RefreshRate = mode.dmDisplayFrequency;
			settings.BitsPerPixel = mode.dmBitsPerPel;
			return true;
		}

		return false;
	}

	void GetSupportedSettings(int index, Array<DisplaySettings>& settings)
	{
		AssertInit();
		HMONITOR hMonitor = monitors->Get(index);

		MONITORINFOEX info;
		info.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &info);

		DEVMODE mode;
		mode.dmSize = sizeof(DEVMODE);
		mode.dmDriverExtra = 0;

		for (int i = 0; EnumDisplaySettings(info.szDevice, i, &mode); i++)
		{
			int j = settings.Add();
			settings[j].Resolution.X = mode.dmPelsWidth;
			settings[j].Resolution.Y = mode.dmPelsHeight;
			settings[j].RefreshRate = mode.dmDisplayFrequency;
			settings[j].BitsPerPixel = mode.dmBitsPerPel;
		}
	}
	
	bool ChangeSettings(int index, const DisplaySettings& settings)
	{
		AssertInit();
		HMONITOR hMonitor = monitors->Get(index);

		MONITORINFOEX info;
		info.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &info);

		DEVMODE mode;
		mode.dmSize = sizeof(DEVMODE);
		mode.dmDriverExtra = 0;
		mode.dmPelsWidth = settings.Resolution.X;
		mode.dmPelsHeight = settings.Resolution.Y;
		mode.dmDisplayFrequency = settings.RefreshRate;
		mode.dmBitsPerPel = settings.BitsPerPixel;
		mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

		return ChangeDisplaySettingsEx(info.szDevice, &mode, NULL, CDS_FULLSCREEN, NULL) == DISP_CHANGE_SUCCESSFUL;
	}
}
