#ifndef __XLI_GL_TEXTURE_2D_H__
#define __XLI_GL_TEXTURE_2D_H__

#include "GLCommon.h"
#include <XliMedia/Bitmap.h>
#include <XliGraphics/Texture.h>

namespace Xli
{
	class GLTexture2D: public Object
	{
		GLuint handle;
		int w, h, mips;

	public:
		static void SetData(GLuint handle, int width, int height, Format format, void* data, bool mipmaps, int* mipCount = 0);
		static void SetData(GLuint handle, Bitmap* bitmap, bool mipmaps, int* width = 0, int* height = 0, int* mipCount = 0);
		static void SetData(GLuint handle, Texture* tex, int* width = 0, int* height = 0, int* mipCount = 0);

		GLTexture2D();
		virtual ~GLTexture2D();

		int GetWidth() const { return w; }
		int GetHeight() const { return h; }
		int GetMipCount() const { return mips; }
		Vector2i GetSize() const { return Vector2i(w, h); }
		GLuint GetHandle() const { return handle; }

		void SetSize(int width, int height);
		void SetData(int width, int height, Format format, void* data, bool mipmaps);
		void SetData(Bitmap* bitmap, bool mipmaps);
		void SetData(Texture* tex);

		void FilterLinear() const;
		void FilterBilinear() const;
		void FilterTrilinear() const;
		void FilterNearest() const;

		void WrapClamp() const;
		void WrapRepeat() const;
	};
}

#endif
