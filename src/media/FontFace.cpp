#include <XliMedia/FontFace.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Xli
{
	class FreeType2FontFace: public FontFace
	{
		int fontSize;
		FT_Face face;
		int ascender, descender;
		Utf32Char activeGlyph;
		Managed<Buffer> buf;

		void setActiveGlyph(Utf32Char c)
		{
			if (c != activeGlyph)
			{
				FT_UInt glyphIndex = FT_Get_Char_Index(face, c);
				FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
				activeGlyph = c;
			}
		}

		int fontUnitsToPixels(FT_Pos units)
		{
			return (int)((float)fontSize * (float)units / (float)face->units_per_EM);
		}

	public:
		FreeType2FontFace(Stream* fontFile, int fontSize)
		{
			static FT_Library library;
			static bool inited = false;

			if (!inited)
			{
				FT_Error error = FT_Init_FreeType(&library);
				if (error) XLI_THROW("Error initializing FreeType 2");
				inited = true;
			}

			buf = Buffer::Create(fontFile->GetLength());
			fontFile->ReadSafe(buf->Data(), 1, buf->Size());

			this->fontSize = fontSize;

			FT_Error error = FT_New_Memory_Face(library, (FT_Byte*)buf->Data(), buf->Size(), 0, &face);
			if (error == FT_Err_Unknown_File_Format) XLI_THROW("Unknown file format");
			else if (error) XLI_THROW("Error loading font");
			error = FT_Set_Pixel_Sizes(face, 0, (FT_UInt)fontSize);
			if (error) XLI_THROW("Error setting font pixel size");

			this->activeGlyph = ~0;
		}
		~FreeType2FontFace()
		{
			FT_Done_Face(face);
		}

		virtual int GetSize()
		{
			return fontSize;
		}

		virtual String GetFamilyName()
		{
			return face->family_name;
		}

		virtual String GetStyleName()
		{
			return face->style_name;
		}

		virtual Vector2i GetBearing(Utf32Char character)
		{
			if (activeGlyph != character)
			{
				activeGlyph = character;
				FT_UInt glyphIndex = FT_Get_Char_Index(face, character);
				int d = FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER);
			}

			return Vector2i(face->glyph->bitmap_left, -face->glyph->bitmap_top);
		}

		virtual Vector2i GetAdvance(Utf32Char character)
		{
			setActiveGlyph(character);
			return Vector2i(face->glyph->advance.x >> 6, face->glyph->advance.y >> 6);
		}

		virtual Bitmap* RenderGlyph(Utf32Char character)
		{
			if (activeGlyph != character)
			{
				activeGlyph = character;
				FT_UInt glyphIndex = FT_Get_Char_Index(face, character);
				int d = FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER);
				if (d != 0) return 0;
			}

			Bitmap* bmp = new Bitmap(face->glyph->bitmap.width, face->glyph->bitmap.rows, FormatL_8_UInt_Normalize);
			memcpy(bmp->GetData(), face->glyph->bitmap.buffer, bmp->GetWidth() * bmp->GetHeight());

			return bmp;
		}
		virtual Vector2i GetKerning(Utf32Char previuousCharacter, Utf32Char character)
		{
			FT_Vector kerning;

			FT_UInt left = FT_Get_Char_Index(face, previuousCharacter);
			FT_UInt right = FT_Get_Char_Index(face, character);

			FT_Get_Kerning(face, left, right, FT_KERNING_DEFAULT, &kerning);

			return Vector2i(kerning.x >> 6, kerning.y >> 6);
		}

		virtual int GetAscender() { return fontUnitsToPixels(face->ascender); }
		virtual int GetDescender() { return -fontUnitsToPixels(face->descender); }
	};

	void FreeType::Init()
	{
	}

	void FreeType::Shutdown()
	{
	}

	FontFace* FreeType::OpenFontFace(Stream* fontFile, int fontSize)
	{
		return new FreeType2FontFace(fontFile, fontSize);
	}
}
