#ifndef __XLI_MEDIA_H__
#define __XLI_MEDIA_H__

/**
    \defgroup XliMedia 
    \brief Media features and file formats

    \defgroup XliMediaFonts Fonts
    \brief TrueType font parsing and rasterization (TTF)
    \ingroup XliMedia

    \defgroup XliMediaTextures Textures
    \brief Loading/saving of images, bitmaps and textures (2D, 3D, Cube) (JPEG, PNG, KTX)
    \ingroup XliMedia

    \defgroup XliMediaCompression Compression
    \brief File and file system compression (GZ, ZIP)
    \ingroup XliMedia

    \defgroup XliMediaMarkup Markup
    \brief Markup parsing and serialization (JSON, XML)
    \ingroup XliMedia
*/

#include <XliMedia/Bitmap.h>
#include <XliMedia/CompressedImage.h>
#include <XliMedia/FontFace.h>
#include <XliMedia/Format.h>
#include <XliMedia/FormattedTextWriter.h>
#include <XliMedia/GZip.h>
#include <XliMedia/Image.h>
#include <XliMedia/ImageReader.h>
#include <XliMedia/ImageWriter.h>
#include <XliMedia/Jpeg.h>
#include <XliMedia/Json.h>
#include <XliMedia/Ktx.h>
#include <XliMedia/Png.h>
#include <XliMedia/ProgressCallback.h>
#include <XliMedia/Texture.h>
#include <XliMedia/Value.h>
#include <XliMedia/Xml.h>
#include <XliMedia/Zip.h>

#endif
