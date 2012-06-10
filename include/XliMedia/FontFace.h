#ifndef __XLI_FONT_FACE_H__
#define __XLI_FONT_FACE_H__

#include <Xli/Object.h>
#include <Xli/File.h>
#include <Xli/Vector2.h>
#include <Xli/String.h>
#include <XliMedia/Bitmap.h>

namespace Xli
{
	class FontFace: public Object
	{
	public:
		virtual int GetAscender() = 0;
		virtual int GetDescender() = 0;
		virtual Vector2i GetBearing(Utf32Char character) = 0;
		virtual Vector2i GetAdvance(Utf32Char character) = 0;
		virtual Vector2i GetKerning(Utf32Char previuousCharacter, Utf32Char character) = 0;
		virtual Bitmap* RenderGlyph(Utf32Char character) = 0;

		virtual String GetFamilyName() = 0;
		virtual String GetStyleName() = 0;
		virtual int GetSize() = 0;
	};

	class FreeType
	{
	public:
		static void Init();
		static void Shutdown();

		static FontFace* OpenFontFace(Stream* fontFile, int size);

		static inline FontFace* OpenFontFace(const String& fileName, int size)
		{
			File f(fileName, FileModeRead);
			return OpenFontFace(&f, size);
		}
	};
}

#endif
