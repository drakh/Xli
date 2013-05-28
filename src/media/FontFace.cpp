#include <XliMedia/FontFace.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Xli
{
	static FT_Library Library;
	static int RefCount = 0;

	class FreeType2FontFace: public FontFace
	{
		int fontSize;
		FT_Face face;
		int ascender, descender;
		UInt32 activeGlyph;
		FontRenderMode activeMode;
		Managed<Buffer> buf;

		void setActiveGlyph(UInt32 c, FontRenderMode mode)
		{
			if (activeGlyph != c || mode != activeMode)
			{
				int m = FT_LOAD_DEFAULT;
				if (mode == FontRenderModeNormal) m |= FT_LOAD_RENDER;
				else if (mode == FontRenderModeMonochrome) m |= FT_LOAD_RENDER | FT_LOAD_MONOCHROME;
				FT_UInt glyphIndex = FT_Get_Char_Index(face, c);
				FT_Load_Glyph(face, glyphIndex, m);
				activeMode = mode;
				activeGlyph = c;
			}
		}

		float fontUnitsToPixels(FT_Pos units)
		{
			return (float)fontSize * (float)units / (float)face->units_per_EM;
		}

	public:
		FreeType2FontFace(Stream* fontFile, int fontSize)
		{
			buf = Buffer::Create(fontFile->GetLength());
			fontFile->ReadSafe(buf->Data(), 1, buf->Size());

			FT_Error error = FT_New_Memory_Face(Library, (FT_Byte*)buf->Data(), buf->Size(), 0, &face);
			if (error == FT_Err_Unknown_File_Format) XLI_THROW("Unknown file format");
			else if (error) XLI_THROW("Error loading font");

			this->activeMode = FontRenderModeNormal;
			this->activeGlyph = ~0;

			SetPixelSize(fontSize);
		}

		virtual ~FreeType2FontFace()
		{
			FT_Done_Face(face);
		}

		virtual void SetPixelSize(int fontSize)
		{
			FT_Error error = FT_Set_Pixel_Sizes(face, 0, (FT_UInt)fontSize);
			if (error) XLI_THROW("Error setting font pixel size");
			this->fontSize = fontSize;
		}

		virtual int GetPixelSize()
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

		virtual Vector2i GetBearing(UInt32 character)
		{
			setActiveGlyph(character, activeMode);
			return Vector2i(face->glyph->bitmap_left, -face->glyph->bitmap_top);
		}

		virtual Bitmap* RenderGlyph(UInt32 character, FontRenderMode mode)
		{
			setActiveGlyph(character, mode);

			int w = face->glyph->bitmap.width;
			int h = face->glyph->bitmap.rows;
			Bitmap* bmp = new Bitmap(w, h, FormatL_8_UInt_Normalize);

			if (mode == FontRenderModeMonochrome)
			{
				UInt8* src = face->glyph->bitmap.buffer;
				UInt8* dst = bmp->GetData();
				int p = face->glyph->bitmap.pitch;

				for (int y = 0; y < h; y++)
					for (int x = 0; x < w; x++)
						*dst++ = ((src[y * p + (x >> 3)] >> (7 - (x & 7))) & 1) * 255;
			}
			else
			{
				memcpy(bmp->GetData(), face->glyph->bitmap.buffer, w * h);
			}

			return bmp;
		}

		virtual Vector2 GetKerning(UInt32 left, UInt32 right)
		{
			FT_Vector kerning;
			FT_Get_Kerning(face, FT_Get_Char_Index(face, left), FT_Get_Char_Index(face, right), FT_KERNING_DEFAULT, &kerning);
			return Vector2((float)kerning.x / 64.0f, (float)kerning.y / 64.0f);
		}

		virtual Vector2 GetAdvance(UInt32 character)
		{
			setActiveGlyph(character, activeMode);
			return Vector2((float)face->glyph->advance.x / 64.0f, (float)face->glyph->advance.y / 64.0f);
		}

		virtual float GetAscender() 
		{ 
			return fontUnitsToPixels(face->ascender); 
		}

		virtual float GetDescender()
		{ 
			return -fontUnitsToPixels(face->descender); 
		}
	};

	void FreeType::Init()
	{
		if (RefCount == 0)
		{
			FT_Error error = FT_Init_FreeType(&Library);
			if (error) XLI_THROW("Error initializing FreeType 2");
		}

		RefCount++;
	}

	void FreeType::Done()
	{
		RefCount--;

		if (RefCount == 0)
		{
			FT_Done_FreeType(Library);
		}
	}

	FontFace* FreeType::OpenFontFace(Stream* fontFile, int fontSize)
	{
		if (RefCount == 0)
		{
			Init();
			atexit(Done);
		}

		return new FreeType2FontFace(fontFile, fontSize);
	}
}
