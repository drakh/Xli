#ifndef __XLI_FONT_FACE_H__
#define __XLI_FONT_FACE_H__

#include <Xli/Object.h>
#include <Xli/File.h>
#include <Xli/Vector2.h>
#include <Xli/String.h>
#include <XliMedia/Bitmap.h>

namespace Xli
{
	enum FontRenderMode
	{
		FontRenderModeNone,
		FontRenderModeNormal,
		FontRenderModeMonochrome,
	};

	class FontFace: public Object
	{
	public:
		virtual Vector2 GetAdvance(Utf32Char c) = 0;
		virtual Vector2 GetKerning(Utf32Char left, Utf32Char right) = 0;
		virtual Vector2i GetBearing(Utf32Char c) = 0;
		virtual Bitmap* RenderGlyph(Utf32Char c, FontRenderMode mode) = 0;

		virtual String GetFamilyName() = 0;
		virtual String GetStyleName() = 0;

		virtual float GetAscender() = 0;
		virtual float GetDescender() = 0;

		virtual void SetPixelSize(int fontSize) = 0;		
		virtual int GetPixelSize() = 0;
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
