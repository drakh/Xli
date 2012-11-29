#ifndef __XLI_GRAPHICS_TEXTURE_H__
#define __XLI_GRAPHICS_TEXTURE_H__

#include <Xli/Array.h>
#include <Xli/Managed.h>
#include <XliMedia/Image.h>

namespace Xli
{
	enum TextureType
	{
		TextureType2D,
		TextureType3D,
		TextureTypeCube,
	};

	class Texture: public Object
	{
	public:
		enum FaceIndex
		{
			FaceIndexPositiveX = 0,
			FaceIndexNegativeX = 1,
			FaceIndexPositiveY = 2,
			FaceIndexNegativeY = 3,
			FaceIndexPositiveZ = 4,
			FaceIndexNegativeZ = 5,
		};

		struct Face
		{
			Array<Managed<Image> > MipLevels;
			
			Face();
			Face(Image* image);

			Face& operator = (const Face& copy);
		};

		TextureType Type;
		int Width, Height, Depth;

		Array<Face> Faces;

		Texture(TextureType type, int width, int height, int depth);

		static Texture* Create2D(Image* img);
		static Texture* CreateCube(Image* posX, Image* negX, Image* posY, Image* negY, Image* posZ, Image* negZ);
		static Texture* Create3D(Image** slices, int depth);
	};
}

#endif
