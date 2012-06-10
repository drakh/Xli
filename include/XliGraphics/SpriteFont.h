#ifndef __XLI_SPRITE_FONT_H__
#define __XLI_SPRITE_FONT_H__

#include "BitmapFont.h"

namespace Xli
{
	enum TextAlign
	{
		TextAlignLeft = 1<<0,
		TextAlignCenter = 1<<1,
		TextAlignRight = 1<<2,
		TextAlignTop = 1<<3,
		TextAlignMiddle = 1<<4,
		TextAlignBottom = 1<<5,

		TextAlignTopLeft = TextAlignTop | TextAlignLeft,
		TextAlignTopCenter = TextAlignTop | TextAlignCenter,
		TextAlignTopRight = TextAlignTop | TextAlignRight,
		TextAlignMiddleLeft = TextAlignMiddle | TextAlignLeft,
		TextAlignMiddleCenter = TextAlignMiddle | TextAlignCenter,
		TextAlignMiddleRight = TextAlignMiddle | TextAlignRight,
		TextAlignBottomLeft = TextAlignBottom | TextAlignLeft,
		TextAlignBottomCenter = TextAlignBottom | TextAlignCenter,
		TextAlignBottomRight = TextAlignBottom | TextAlignRight,
	};

	class SpriteFont: public Object
	{
	protected:
		SpriteFont(BitmapFont* bmpFont);

	public:
		struct Vertex
		{
			Vector2i16 Position;
			Vector2u16 TexCoord;
		};

		struct CharacterData
		{
			Vector2u16 UpperLeft, UpperRight, LowerRight, LowerLeft;
			HashMap<Utf32Char, int> Kerning;
			Vector2i Size, Bearing;
			int Advance;
		};

		int Ascender, Descender, SpaceWidth;
		HashMap<Utf32Char, CharacterData> Characters;
		

		virtual ~SpriteFont() {}

		int GetStringWidth(const String& str) const;
		int GetLineHeight() const;

		void WriteCharacter(const CharacterData& cd, const Vector2i& cursor, Array<Vertex>& output) const;
		Vector2i WriteString(const String& str, Vector2i cursor, const Recti& cullRect, Array<Vertex>& output) const;

		void WriteString(const String& str, const Recti& rect, int textAlign, bool wordWrap, Array<Vertex>& output) const;
		void WriteString(const String& str, const Recti& rect, int textAlign, Array<Vertex>& output) const { WriteString(str, rect, textAlign, false, output); }

		void WordWrap(const String& str, Vector2i windowSize, Array<String>& result, int startPosition = 0, int* endPosition = 0, int startIndex = 0, int* endIndex = 0) const;
	};
}

#endif
