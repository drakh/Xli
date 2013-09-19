#ifndef __XLI_FONT_FACE_H__
#define __XLI_FONT_FACE_H__

#include <Xli/Object.h>
#include <Xli/File.h>
#include <Xli/Vector2.h>
#include <Xli/String.h>
#include <XliMedia/Bitmap.h>

namespace Xli
{
	/**
		\ingroup XliMediaFonts
	*/
	enum FontRenderMode
	{
		FontRenderModeNone,
		FontRenderModeNormal,
		FontRenderModeMonochrome,
	};

	/**
		\ingroup XliMediaFonts
	*/
	class FontFace: public Object
	{
	public:
		virtual String GetFamilyName() = 0;
		virtual String GetStyleName() = 0;

		virtual float GetAscender(float pixelSize) = 0;
		virtual float GetDescender(float pixelSize) = 0;
		virtual float GetLineHeight(float pixelSize) = 0;

		virtual bool HasGlyph(float pixelSize, Utf32 c) = 0;
		virtual Bitmap* RenderGlyph(float pixelSize, Utf32 c, FontRenderMode mode, Vector2* outAdvance, Vector2* outBearing) = 0;

		virtual bool TryGetKerning(float pixelSize, Utf32 left, Utf32 right, Vector2* outKerning) = 0;
	};

	/**
		\ingroup XliMediaFonts
	*/
	class FreeType
	{
	public:
		static void Init();
		static void Done();

		static FontFace* LoadFontFace(Stream* fontFile);

		static FontFace* LoadFontFace(const String& filename)
		{
			File f(filename, FileModeRead);
			return LoadFontFace(&f);
		}
	};
}

#endif
