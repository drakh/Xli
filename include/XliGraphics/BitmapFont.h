#ifndef __XLI_GRAPHICS_BITMAP_FONT_H__
#define __XLI_GRAPHICS_BITMAP_FONT_H__

#include <XliMedia/Bitmap.h>
#include <Xli/Managed.h>
#include <Xli/Vector2.h>
#include <Xli/HashMap.h>
#include <Xli/File.h>

namespace Xli
{
	class BitmapFont: public Object
	{
	public:
		struct CharacterData
		{
			Vector2i UpperLeft, UpperRight, LowerRight, LowerLeft;
			HashMap<Utf32Char, int> Kerning;
			Vector2i Bearing, Size;
			int Advance;
		};

		int Ascender, Descender, SpaceWidth;
		HashMap<Utf32Char, CharacterData> Characters;
		Managed<Xli::Bitmap> Bitmap;

		static BitmapFont* Load(Stream* stream);
		void Save(Stream* stream) const;

		static inline BitmapFont* Load(const String& fileName)
		{
			File f(fileName, FileModeRead);
			return Load(&f);
		}

		inline void Save(const String& fileName) const
		{
			File f(fileName, FileModeWrite);
			Save(&f);
		}
	};
}

#endif
