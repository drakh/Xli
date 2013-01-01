#ifndef __XLI_GRAPHICS_TEXTURE_H__
#define __XLI_GRAPHICS_TEXTURE_H__

#include <Xli/Array.h>
#include <Xli/Managed.h>
#include <XliMedia/Image.h>

namespace Xli
{
	/**
		\ingroup Textures
	*/
	enum TextureType
	{
		TextureType2D,
		TextureType3D,
		TextureTypeCube,
	};

	/**
		\ingroup Textures
	*/
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
		Array<Face> Faces;

		int GetWidth() const;
		int GetHeight() const;
		int GetDepth() const;
		int GetMipCount() const;
		Format GetFormat() const;

		Texture(TextureType type);

		static Texture* Create(Image* img);
		static Texture* CreateCube(Image* posX, Image* negX, Image* posY, Image* negY, Image* posZ, Image* negZ);

		enum Cube2DMode
		{
			Cube2DMode1x6,
			Cube2DMode2x3,
			Cube2DMode3x2,
			Cube2DMode6x1,
		};

		Texture* Convert2DToCube();
		Texture* ConvertCubeTo2D(Cube2DMode mode);
	};
}

#endif
