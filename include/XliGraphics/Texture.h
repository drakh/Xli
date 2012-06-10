#ifndef __XLI_GRAPHICS_TEXTURE_H__
#define __XLI_GRAPHICS_TEXTURE_H__

#include <Xli/Array.h>
#include <Xli/Managed.h>
#include <XliMedia/Image.h>

namespace Xli
{
	class Texture: public Object
	{
	public:
		class Face
		{
		public:
			enum FaceType
			{
				FaceTypeUnknown,
				FaceTypePositiveX,
				FaceTypeNegativeX,
				FaceTypePositiveY,
				FaceTypeNegativeY,
				FaceTypePositiveZ,
				FaceTypeNegativeZ,
			};

			FaceType Type;
			Array<Managed<Image> > MipLevels;

			Face();
		};

		enum TextureType
		{
			TextureType2D,
			TextureTypeCubeMap,
		};

		TextureType Type;
		Array<Face> Faces;

		Texture(TextureType type);
		Texture(Image* img);
	};
}

#endif
