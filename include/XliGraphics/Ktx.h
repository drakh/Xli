#ifndef __XLI_GRAPHICS_KTX_H__
#define __XLI_GRAPHICS_KTX_H__

#include "Texture.h"
#include <Xli/File.h>

namespace Xli
{
	class Ktx
	{
	public:
		static void Save(Stream* output, Texture* tex);
		static void Save(const String& fileName, Texture* tex);
		static Texture* Load(Stream* input);
		static Texture* Load(const String& fileName);
	};
}


#endif
