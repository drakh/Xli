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
		\ingroup Fonts
	*/
	enum FontRenderMode
	{
		FontRenderModeNone,
		FontRenderModeNormal,
		FontRenderModeMonochrome,
	};

	/**
		\ingroup Fonts
	*/
	class FontFace: public Object
	{
	public:
		virtual Vector2 GetAdvance(UInt32 c) = 0;
		virtual Vector2 GetKerning(UInt32 left, UInt32 right) = 0;
		virtual Vector2i GetBearing(UInt32 c) = 0;
		virtual Bitmap* RenderGlyph(UInt32 c, FontRenderMode mode) = 0;

		virtual String GetFamilyName() = 0;
		virtual String GetStyleName() = 0;

		virtual float GetAscender() = 0;
		virtual float GetDescender() = 0;

		virtual void SetPixelSize(int fontSize) = 0;		
		virtual int GetPixelSize() = 0;
	};

	/**
		\ingroup Fonts
	*/
	class FreeType
	{
	public:
		static void Init();
		static void Done();

		static FontFace* OpenFontFace(Stream* fontFile, int size);

		static inline FontFace* OpenFontFace(const String& filename, int size)
		{
			File f(filename, FileModeRead);
			return OpenFontFace(&f, size);
		}
	};
}

#endif
