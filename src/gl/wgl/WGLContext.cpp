#include <XliGL.h>
#include <Xli/PlatformSpecific/Win32Window.h>
#include <Xli/PlatformSpecific/Win32Helpers.h>
#include <Xli/Shared.h>
#include "../3rdparty/glew/include/GL/wglew.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        static bool Inited = false;

        class WGLContext : public GLContext
        {
            HDC hDC;
            HGLRC hGLRC;
            Shared<Win32Window> window;
            PIXELFORMATDESCRIPTOR pfd;
            int pf;

            int TryEnableMultisample(const GLContextAttributes& attribs)
            {
                if (!Inited)
                {
                    static const PIXELFORMATDESCRIPTOR tmpPfd =
                    {
                        sizeof(PIXELFORMATDESCRIPTOR),
                        1,
                        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                        PFD_TYPE_RGBA,
                        32,
                        0, 0, 0, 0, 0, 0,
                        0, // No alpha buffer
                        0,
                        0, // No accumulation buffer
                        0, 0, 0, 0,
                        24, // 24-bit z buffer
                        0, // No stencil buffer
                        0, // No auxilary buffer
                        PFD_MAIN_PLANE,
                        0, 0, 0, 0
                    };

                    HWND tmpWnd = CreateWindowEx(0, L"STATIC", L"", 0, 0, 0, 16, 16, window->GetHWND(), 0, GetModuleHandle(0), 0);
                    HDC tmpDC = GetDC(tmpWnd);
                    int fmt = ChoosePixelFormat(tmpDC, &tmpPfd);
                    SetPixelFormat(tmpDC, fmt, &pfd);
                    HGLRC tmpGL = wglCreateContext(tmpDC);
                    wglMakeCurrent(tmpDC, tmpGL);

                    glewInit();

                    wglMakeCurrent(tmpDC, 0);
                    wglDeleteContext(tmpGL);
                    ReleaseDC(tmpWnd, tmpDC);
                    DestroyWindow(tmpWnd);

                    Inited = true;
                }

                if (attribs.Samples <= 1)
                {
                    return -1;
                }

                int iattribs[] =
                {
                    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                    WGL_RED_BITS_ARB, attribs.ColorBits.R,
                    WGL_GREEN_BITS_ARB, attribs.ColorBits.G,
                    WGL_BLUE_BITS_ARB, attribs.ColorBits.B,
                    WGL_ALPHA_BITS_ARB, attribs.ColorBits.A,
                    WGL_ACCUM_RED_BITS_ARB, attribs.AccumBits.R,
                    WGL_ACCUM_GREEN_BITS_ARB, attribs.AccumBits.G,
                    WGL_ACCUM_BLUE_BITS_ARB, attribs.AccumBits.B,
                    WGL_ACCUM_ALPHA_BITS_ARB, attribs.AccumBits.A,
                    WGL_DEPTH_BITS_ARB, attribs.DepthBits,
                    WGL_STENCIL_BITS_ARB, attribs.StencilBits,
                    WGL_DOUBLE_BUFFER_ARB, attribs.Buffers > 1 ? GL_TRUE : GL_FALSE,
                    WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
                    WGL_SAMPLES_ARB, attribs.Samples,
                    0, 0
                };

                // TODO: attribs.Stereo is not handled.

                float fattribs[] = { 0, 0 };

                UINT numConfigs;
                int configs[64];

                while (true)
                {
                    if (!wglChoosePixelFormatARB(hDC, iattribs, fattribs, 64, configs, &numConfigs) || numConfigs == 0)
                    {
                        if (iattribs[19] > 2)
                        {
                            iattribs[19] /= 2;
                            continue;
                        }

                        ErrorPrintLine("WGL WARNING: Unable to find any multisampled OpenGL formats");
                        return -1;
                    }

                    break;
                }

                if (!SetPixelFormat(hDC, configs[0], &pfd))
                {
                    ErrorPrintLine("WGL ERROR: Unable to set multisampled OpenGL format: " + Win32Helpers::GetLastErrorString());
                    return -1;
                }

                return configs[0];
            }

        public:
            WGLContext(Win32Window* wnd, const GLContextAttributes& attribs)
            {
                this->window = wnd;
                hDC = GetDC(wnd->GetHWND());

                ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
                pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
                pfd.nVersion = 1;
                pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;

                if (attribs.Buffers > 1)
                    pfd.dwFlags |= PFD_DOUBLEBUFFER;

                if (attribs.Stereo)
                    pfd.dwFlags |= PFD_STEREO;

                pfd.iPixelType = PFD_TYPE_RGBA;
                pfd.cRedBits = attribs.ColorBits.R;
                pfd.cGreenBits = attribs.ColorBits.G;
                pfd.cBlueBits = attribs.ColorBits.B;
                pfd.cAlphaBits = attribs.ColorBits.A;
                pfd.cAccumRedBits = attribs.AccumBits.R;
                pfd.cAccumGreenBits = attribs.AccumBits.G;
                pfd.cAccumBlueBits = attribs.AccumBits.B;
                pfd.cAccumAlphaBits = attribs.AccumBits.A;
                pfd.cDepthBits = attribs.DepthBits;
                pfd.cStencilBits = attribs.StencilBits;
                pfd.iLayerType = PFD_MAIN_PLANE;

                pf = TryEnableMultisample(attribs);

                if (pf == -1 && !SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd))
                    XLI_THROW("Failed to create OpenGL context: " + Win32Helpers::GetLastErrorString());

                hGLRC = wglCreateContext(hDC);
                
                if (!hGLRC)
                    XLI_THROW("Failed to create OpenGL context: " + Win32Helpers::GetLastErrorString());

                MakeCurrent(true);

                glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                SwapBuffers();

                if (pf != -1)
                    glEnable(GL_MULTISAMPLE_ARB);
            }

            WGLContext(WGLContext* srcCtx)
            {
                hDC = srcCtx->hDC;
                pf = srcCtx->pf;

                hGLRC = wglCreateContext(hDC);

                if (!hGLRC)
                    XLI_THROW("Unable to create shared OpenGL context: " + Win32Helpers::GetLastErrorString());

                if (!wglMakeCurrent(hDC, 0))
                    XLI_THROW("Unable to make OpenGL context no longer current: " + Win32Helpers::GetLastErrorString());

                if (!wglShareLists(srcCtx->hGLRC, hGLRC))
                    XLI_THROW("Unable to share OpenGL contexts: " + Win32Helpers::GetLastErrorString());

                srcCtx->MakeCurrent(true);
            }

            virtual ~WGLContext()
            {
                wglDeleteContext(hGLRC);
            }

            virtual GLContext* CreateSharedContext()
            {
                return new WGLContext(this);
            }

            virtual void SetWindow(Window* window)
            {
                if (window->GetImplementation() != WindowImplementationWin32) 
                    XLI_THROW("Unsupported window");
                
                if (window->GetNativeHandle() == window->GetNativeHandle())
                    return;

                this->window = (Win32Window*)window;
                this->hDC = GetDC(this->window->GetHWND());

                if (pf != -1)
                    SetPixelFormat(hDC, pf, &pfd);
                else
                    SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

                MakeCurrent(true);

                if (pf != -1)
                    glEnable(GL_MULTISAMPLE_ARB);
            }

            virtual void MakeCurrent(bool current)
            {
                if (!wglMakeCurrent(hDC, current ? hGLRC : 0))
                    XLI_THROW("Unable to make OpenGL context current: " + Win32Helpers::GetLastErrorString());
            }

            virtual void SwapBuffers()
            {
                ::SwapBuffers(hDC);
            }

            virtual bool SetSwapInterval(int interval)
            {
                return wglSwapIntervalEXT && wglSwapIntervalEXT(interval) == TRUE;
            }

            virtual int GetSwapInterval()
            {
                return wglGetSwapIntervalEXT ? wglGetSwapIntervalEXT() : 0;
            }

            virtual Vector2i GetBackbufferSize()
            {
                return window->GetClientSize();
            }

            virtual int GetSamples()
            {
                int attr = WGL_SAMPLES_ARB;
                int samples = 0;
                wglGetPixelFormatAttribivARB(hDC, pf, 0, 1, &attr, &samples);
                return samples;
            }
        };
    }

    GLContext* GLContext::Create(Window* window, const GLContextAttributes& attribs)
    {
        if (window->GetImplementation() != WindowImplementationWin32) 
            XLI_THROW("Unsupported window implementation");
        
        return new PlatformSpecific::WGLContext((PlatformSpecific::Win32Window*)window, attribs);
    }
}
