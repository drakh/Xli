#include <XliGL/GLHelpers.h>

#include <Xli/Text.h>
#include <XliMedia/Jpeg.h>
#include <XliMedia/Png.h>
#include <XliGraphics/BitmapFontBuilder.h>
#include <XliGraphics/Semantic.h>

namespace Xli
{
	GLProgram* GLHelpers::LoadProgram(const String& vertFn, const String& fragFn)
	{
		CharString vertSrc = Text::Load(vertFn);
		CharString fragSrc = Text::Load(fragFn);
		return new GLProgram(vertFn, vertSrc, fragFn, fragSrc);
	}

	GLProgram* GLHelpers::LoadProgram(const String& fnPrefix)
	{
		return LoadProgram(fnPrefix + ".vert", fnPrefix + ".frag");
	}

	GLTexture2D* GLHelpers::LoadTexture2D(const String& fn, bool mips)
	{
		File f(fn, FileModeRead);
		Managed<ImageReader> ir;

		if (fn.ToLower().EndsWith(".png")) ir = Png::CreateReader(&f);
		else if (fn.ToLower().EndsWith(".jpg")) ir = Jpeg::CreateReader(&f);
		else if (fn.ToLower().EndsWith(".jpeg")) ir = Jpeg::CreateReader(&f);
		else XLI_THROW("Unsupported file extension: " + fn);

		Managed<Buffer> buf = Buffer::Create(ir->GetBufferSize());
		ir->Read(buf->Data());

		GLTexture2D* tex = new GLTexture2D();
		tex->SetData(ir->GetWidth(), ir->GetHeight(), ir->GetFormat(), buf->Data(), mips);

		if (mips) tex->FilterTrilinear();
		else tex->FilterLinear();

		return tex;
	}

	GLFont* GLHelpers::LoadFont(const String& fn, int fontSize, const String& charSet, bool pow2)
	{
	    BitmapFontBuilder bfb;
	    bfb.PowerOfTwo = pow2;
		if (charSet.Length() > 0) bfb.CharSet = charSet;
		Managed<BitmapFont> bf = bfb.Build(fn, fontSize);
		return new GLFont(bf);
	}

	void GLHelpers::DrawQuad(GLProgram* prog)
	{
		static const GLbyte vdata[] = { 0,0, 0,1, 1,1, 0,0, 1,1, 1,0 };

		GLint pos = prog->GetAttribLocation(Semantic::Position);

		if (pos != -1)
		{
			glEnableVertexAttribArray(pos);
			glVertexAttribPointer(pos, 2, GL_BYTE, GL_FALSE, 2*sizeof(GLbyte), vdata);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisableVertexAttribArray(pos);
			GLCheckError();
		}
	}
}
