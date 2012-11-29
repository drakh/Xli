#include <XliGraphics/Texture.h>

namespace Xli
{
	Texture::Face::Face()
	{
	}

	Texture::Face::Face(Image* img)
	{
		MipLevels.Add(Share(img));
	}

	Texture::Face& Texture::Face::operator = (const Face& copy)
	{
		MipLevels.Clear();
		MipLevels.Add(copy.MipLevels);
		return *this;
	}

	Texture::Texture(TextureType type, int width, int height, int depth)
	{
		Type = type;
		Width = width;
		Height = height;
		Depth = depth;
	}

	Texture* Texture::Create2D(Image* img)
	{
		Texture* result = new Texture(TextureType2D, img->GetWidth(), img->GetHeight(), 0);
		result->Faces.Add(Texture::Face(img));
		return result;
	}

	Texture* Texture::CreateCube(Image* posX, Image* negX, Image* posY, Image* negY, Image* posZ, Image* negZ)
	{
		Texture* result = new Texture(TextureTypeCube, posX->GetWidth(), posX->GetWidth(), posX->GetWidth());
		result->Faces.Reserve(6);
		result->Faces.Add(Texture::Face(posX));
		result->Faces.Add(Texture::Face(negX));
		result->Faces.Add(Texture::Face(posY));
		result->Faces.Add(Texture::Face(negY));
		result->Faces.Add(Texture::Face(posZ));
		result->Faces.Add(Texture::Face(negZ));
		return result;
	}

	Texture* Texture::Create3D(Image** slices, int depth)
	{
		Texture* result = new Texture(TextureType3D, slices[0]->GetWidth(), slices[1]->GetWidth(), depth);
		result->Faces.Reserve(depth);
		for (int i = 0; i < depth; i++)
			result->Faces.Add(Texture::Face(slices[i]));
		return result;
	}
}
