#ifndef __XLI_GRAPHICS_KTX_H__
#define __XLI_GRAPHICS_KTX_H__

#include <Xli/File.h>
#include <XliMedia/Texture.h>

namespace Xli
{
    /**
        \ingroup XliMediaTextures
    */
    class Ktx
    {
    public:
        static void Save(Stream* output, Texture* tex);
        static void Save(const String& filename, Texture* tex);
        static Texture* Load(Stream* input);
        static Texture* Load(const String& filename);
    };
}


#endif
