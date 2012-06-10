#ifndef __XLI_GRAPHICS_ATLAS_BUILDER_H__
#define __XLI_GRAPHICS_ATLAS_BUILDER_H__

#include "Atlas.h"

namespace Xli
{
	class AtlasBuilder
	{
	public:
		enum Flag
		{
			FlagPremultiplyAlpha = 1<<0
		};

		struct Tile
		{
			Shared<Bitmap> Source;
			//Bitmap::Components Components;
			int Flags;

			Tile() {}
			Tile(Bitmap* src, int flags): Source(src), Flags(flags) {}
		};

		int Padding;
		bool AllowRotation;
		bool PowerOfTwo;
		Format PixelFormat;
		Array<Tile> Tiles;

		AtlasBuilder();

		int AddTile(Bitmap* src, int flags = 0) { return Tiles.Add(Tile(src, flags)); }

		BitmapAtlas* BuildBitmapAtlas();
	};
}


#endif
