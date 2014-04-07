#ifndef __XLI_GL_GL_CONTEXT_H__
#define __XLI_GL_GL_CONTEXT_H__

#include <Xli/Window.h>

namespace Xli
{
    class GLContextAttributes
    {
    public:
        Vector4i ColorBits;
        int DepthBits;
        int StencilBits;
        int Samples;
        Vector4i AccumBits;
        int Buffers;
        bool Stereo;

        GLContextAttributes(const Vector4i& colorBits, int depthBits, int stencilBits, int samples, const Vector4i& accumBits, int buffers, bool stereo);

        static const GLContextAttributes& Default();
    };

    /**
        \ingroup XliGL
     */
    class GLContext: public Object
    {
    public:
        static GLContext* Create(Window* window, const GLContextAttributes& attribs);

        virtual ~GLContext() {}

        virtual GLContext* CreateSharedContext() = 0;

        virtual void SetWindow(Window* window) = 0;

        virtual void MakeCurrent(bool current) = 0;
        virtual void SwapBuffers() = 0;

        /**
            Sets the swap interval.
            @param interval 1 enables VSync, 0 disables VSync
            @return true on success, false on failure
        */
        virtual bool SetSwapInterval(int interval) = 0;

        virtual int GetSwapInterval() = 0;

        virtual Vector2i GetBackbufferSize() = 0;

        virtual int GetSamples() = 0;
    };
}

#endif
