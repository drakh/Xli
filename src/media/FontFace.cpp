#include <XliMedia/FontFace.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Xli
{
	static FT_Library FreeTypeLibrary;
	static int FreeTypeRefCount = 0;

	class FreeTypeFontFace: public FontFace
	{
		Managed<Buffer> _buf;
		FT_Face _face;

		float FontUnitsToPixels(float pixelSize, FT_Pos units)
		{
			return pixelSize * (float)units / (float)_face->units_per_EM;
		}

		void SetPixelSize(float pixelSize)
		{
			if (FT_Set_Pixel_Sizes(_face, 0, (FT_UInt)pixelSize))
				XLI_THROW("Error setting font face pixel size");
		}

	public:
		FreeTypeFontFace(Stream* fontFile)
		{
			_buf = Buffer::Create(fontFile->GetLength());
			fontFile->ReadSafe(_buf->Data(), 1, _buf->Size());

			switch (FT_New_Memory_Face(FreeTypeLibrary, (FT_Byte*)_buf->Data(), _buf->Size(), 0, &_face))
			{
			case 0:
				break;

			case FT_Err_Unknown_File_Format:
				XLI_THROW("Error loading font: Unknown file format");
				break;

			default:
				XLI_THROW("Error loading font");
				break;
			}
		}

		virtual ~FreeTypeFontFace()
		{
			FT_Done_Face(_face);
		}

		virtual String GetFamilyName()
		{
			return _face->family_name;
		}

		virtual String GetStyleName()
		{
			return _face->style_name;
		}

		virtual float GetAscender(float pixelSize) 
		{ 
			return FontUnitsToPixels(pixelSize, _face->ascender);
		}

		virtual float GetDescender(float pixelSize)
		{ 
			return -FontUnitsToPixels(pixelSize, _face->descender);
		}

		virtual float GetLineHeight(float pixelSize)
		{
			return FontUnitsToPixels(pixelSize, _face->height);
		}

		virtual bool HasGlyph(float pixelSize, UInt32 c)
		{
			return true;
		}

		virtual Bitmap* RenderGlyph(float pixelSize, UInt32 c, FontRenderMode mode, Vector2* outAdvance, Vector2* outBearing)
		{
			SetPixelSize(pixelSize);

			Bitmap* result = 0;
			FT_UInt glyphIndex = FT_Get_Char_Index(_face, c);

			int m = FT_LOAD_DEFAULT;
			
			switch (mode) 
			{
			case FontRenderModeNormal:
				m |= FT_LOAD_RENDER;
				break;

			case FontRenderModeMonochrome:
				m |= FT_LOAD_RENDER | FT_LOAD_MONOCHROME;
				break;
			}

			FT_Load_Glyph(_face, glyphIndex, m);

			if (mode != FontRenderModeNone)
			{
				int w = _face->glyph->bitmap.width;
				int h = _face->glyph->bitmap.rows;
				result = new Bitmap(w, h, FormatL_8_UInt_Normalize);

				if (mode == FontRenderModeMonochrome)
				{
					UInt8* src = _face->glyph->bitmap.buffer;
					UInt8* dst = result->GetData();
					int p = _face->glyph->bitmap.pitch;

					for (int y = 0; y < h; y++)
						for (int x = 0; x < w; x++)
							*dst++ = ((src[y * p + (x >> 3)] >> (7 - (x & 7))) & 1) * 255;
				}
				else
				{
					memcpy(result->GetData(), _face->glyph->bitmap.buffer, w * h);
				}
			}

			if (outAdvance)
				*outAdvance = Vector2((float)_face->glyph->advance.x / 64.0f, (float)_face->glyph->advance.y / 64.0f);

			if (outBearing)
				*outBearing = Vector2((float)_face->glyph->bitmap_left, (float)-_face->glyph->bitmap_top);

			return result;
		}

		virtual bool TryGetKerning(float pixelSize, UInt32 left, UInt32 right, Vector2* outKerning)
		{
			SetPixelSize(pixelSize);

			FT_Vector kerning;
			if (!FT_Get_Kerning(_face, FT_Get_Char_Index(_face, left), FT_Get_Char_Index(_face, right), FT_KERNING_DEFAULT, &kerning))
			{
				if (outKerning)
					*outKerning = Vector2((float)kerning.x / 64.0f, (float)kerning.y / 64.0f);
				
				return true;
			}
			
			return false;
		}
	};

	void FreeType::Init()
	{
		if (FreeTypeRefCount == 0 && FT_Init_FreeType(&FreeTypeLibrary) != 0)
			XLI_THROW("Error initializing FreeType");

		FreeTypeRefCount++;
	}

	void FreeType::Done()
	{
		FreeTypeRefCount--;

		if (FreeTypeRefCount == 0)
			FT_Done_FreeType(FreeTypeLibrary);
	}

	FontFace* FreeType::LoadFontFace(Stream* fontFile)
	{
		if (FreeTypeRefCount == 0)
		{
			Init();
			atexit(Done);
		}

		return new FreeTypeFontFace(fontFile);
	}
}
