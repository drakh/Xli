#include <XliGraphics/Texture.h>

namespace Xli
{
	Texture::Face::Face()
	{
		Type = FaceTypeUnknown;
	}

	Texture::Texture(TextureType type)
	{
		Type = type;
	}

	Texture::Texture(Image* img)
	{
		Type = TextureType2D;
		Faces.Add();
		Faces[0].MipLevels.Add(Share(img));
	}
}
