#ifndef __XLI_PLATFORM_DISPLAY_H__
#define __XLI_PLATFORM_DISPLAY_H__

#include <Xli/Rectangle.h>

namespace Xli
{
	/**
		\ingroup WM
	*/
	struct DisplaySettings
	{
		Vector2i Resolution;
		int RefreshRate;
		int BitsPerPixel;
	};

	/**
		\ingroup WM
	*/
	class Display
	{
	public:
		static void Init();
		static void Shutdown();

		/**
			Returns the number of screens connected to the system.
		*/
		static int GetCount();

		/**
			Returns the coordinate system of the specified screen.
		*/
		static Recti GetRect(int index);

		static void GetCurrentSettings(int index, DisplaySettings& settings);
		static void GetSupportedSettings(int index, Array<DisplaySettings>& settings);
		static bool ChangeSettings(int index, const DisplaySettings& settings);
	};
}


#endif
