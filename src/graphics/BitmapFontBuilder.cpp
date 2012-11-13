#include <XliGraphics/BitmapFontBuilder.h>
#include <XliGraphics/AtlasBuilder.h>
#include <Xli/Console.h>
#include <XliMedia/Png.h>

namespace Xli
{
	BitmapFontBuilder::BitmapFontBuilder()
	{
		Padding = 4;
		AllowRotation = true;
		PowerOfTwo = false;
		PixelFormat = FormatL_8_UInt_Normalize;
		CharSet = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	}

	BitmapFont* BitmapFontBuilder::Build(FontFace* ff)
	{
		AtlasBuilder cb;
		cb.PixelFormat = PixelFormat;
		cb.Padding = Padding;
		cb.AllowRotation = AllowRotation;
		cb.PowerOfTwo = PowerOfTwo;

		for (int i = 0; i < CharSet.Length(); i++)
		{
			Utf32Char c = CharSet[i];
			if (c < 0xff && isspace(c)) continue;

			Managed<Bitmap> bmp = ff->RenderGlyph(c, FontRenderModeNormal);
			cb.AddTile(bmp);
		}

		Managed<BitmapAtlas> Atlas = cb.BuildBitmapAtlas();
		Managed<BitmapFont> bf = new BitmapFont();
		int AtlasIndex = 0;

		for (int i = 0; i < CharSet.Length(); i++)
		{
			Utf32Char c = CharSet[i];

			if (c < 0xff && isspace(c))
			{
				continue;
			}
			else if (bf->Characters.ContainsKey(c))
			{
				ErrorPrintLine("BitmapFontBuilder WARNING: Character already added: " + String((Utf16Char)c));
				continue;
			}

			BitmapFont::CharacterData cd;

			cd.UpperRight = Atlas->Tiles[AtlasIndex].UpperRight;
			cd.UpperLeft = Atlas->Tiles[AtlasIndex].UpperLeft;
			cd.LowerLeft = Atlas->Tiles[AtlasIndex].LowerLeft;
			cd.LowerRight = Atlas->Tiles[AtlasIndex].LowerRight;
			cd.Size.X = cd.UpperRight.X - cd.UpperLeft.X;
			cd.Size.Y = cd.LowerRight.Y - cd.UpperRight.Y;

			if (Atlas->Tiles[AtlasIndex].IsRotated)
			{
				cd.Size.X = cd.UpperRight.Y - cd.UpperLeft.Y;
				cd.Size.Y = cd.UpperRight.X - cd.LowerRight.X;
			}

			AtlasIndex++;

			for (int j = 0; j < CharSet.Length(); j++)
			{
				if (j != i)
				{
					int k = (int)ff->GetKerning(CharSet[j], c).X;

					if (k != 0 && !cd.Kerning.ContainsKey(CharSet[j]))
					{
						cd.Kerning.Add(CharSet[j], k);
					}
				}
			}

			cd.Bearing = ff->GetBearing(CharSet[i]);
			cd.Advance = (int)ff->GetAdvance(CharSet[i]).X;

			bf->Characters.Add(c, cd);
		}

		bf->Bitmap = Atlas->Bitmap;
		bf->Ascender = (int)ff->GetAscender();
		bf->Descender = (int)ff->GetDescender();
		bf->SpaceWidth = (int)ff->GetAdvance(' ').X;

		return Share(bf.Pointer());
	}

	BitmapFont* BitmapFontBuilder::Build(const Xli::String& fileName, int size)
	{
		Managed<FontFace> ff = FreeType::OpenFontFace(fileName, size);
		return Build(ff);
	}
}
