#include <Xli/Display.h>
#include <Xli/PlatformSpecific/Win32Header.h>
#include <Xli/Array.h>
#include <cstdlib>

namespace Xli
{
	static Array<HMONITOR>* Monitors = 0;
	static int InitCount = 0;

	static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
	{
		((Array<HMONITOR>*)dwData)->Add(hMonitor);
		return TRUE;
	}

	void DisplayInit()
	{
		if (!InitCount)
		{
			Monitors = new Array<HMONITOR>();
			EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)Monitors);
		}

		InitCount++;
	}

	void DisplayDone()
	{
		InitCount--;

		if (InitCount == 0)
		{
			Monitors->Release();
			Monitors = 0;
		}
		else if (InitCount < 0)
		{
			XLI_THROW_BAD_DELETE;
		}
	}

	static void AssertInit()
	{
		if (!InitCount)
		{
			DisplayInit();
			atexit(DisplayDone);
		}
	}

	int Display::GetCount()
	{
		AssertInit();
		return Monitors->Length();
	}

	Recti Display::GetRect(int index)
	{
		AssertInit();
		HMONITOR hMonitor = Monitors->Get(index);

		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(hMonitor, &info);

		return Recti(info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom);
	}

	bool Display::GetCurrentSettings(int index, DisplaySettings& settings)
	{
		AssertInit();
		HMONITOR hMonitor = Monitors->Get(index);

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

	void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings)
	{
		AssertInit();
		HMONITOR hMonitor = Monitors->Get(index);

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
	
	bool Display::ChangeSettings(int index, const DisplaySettings& settings)
	{
		AssertInit();
		HMONITOR hMonitor = Monitors->Get(index);

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
