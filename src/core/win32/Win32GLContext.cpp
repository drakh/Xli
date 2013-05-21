#include <XliGL.h>
#include <Xli/GLContext.h>
#include <Xli/Win32Window.h>
#include <Xli/Win32Helpers.h>
#include "../3rdparty/glew/include/GL/wglew.h"

namespace Xli
{
	static const PIXELFORMATDESCRIPTOR pfd =
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

	static bool inited = false;

	class Win32GLContext: public GLContext
	{
		HDC hDC;
		HGLRC ctx;
		Shared<Win32Window> wnd;
		int pf;
		
		int TryEnableMultisample(int multiSamples)
		{
			if (!inited)
			{
				HWND tmpW = CreateWindowEx(0, L"STATIC", L"", 0, 0, 0, 16, 16, wnd->GetHWND(), 0, GetModuleHandle(0), 0);
				HDC tmpDC = GetDC(tmpW);
				int fmt = ChoosePixelFormat(tmpDC, &pfd);
				SetPixelFormat(tmpDC, fmt, &pfd);
				HGLRC tmpGL = wglCreateContext(tmpDC);
				wglMakeCurrent(tmpDC, tmpGL);
				
				glewInit();

				wglMakeCurrent(tmpDC, 0);
				wglDeleteContext(tmpGL);
				ReleaseDC(tmpW, tmpDC);
				DestroyWindow(tmpW);

				inited = true;
			}

			if (multiSamples <= 1)
			{
				return -1;
			}

			int attribs[] =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_COLOR_BITS_ARB, 24,
				WGL_ALPHA_BITS_ARB, 0,
				WGL_DEPTH_BITS_ARB, 16,
				WGL_STENCIL_BITS_ARB, 0,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
				WGL_SAMPLES_ARB, multiSamples,
				0, 0
			};

			float fattribs[] = { 0, 0 };

			UINT numConfigs;
			int configs[64];

			while (true)
			{
				if (!wglChoosePixelFormatARB(hDC, attribs, fattribs, 64, configs, &numConfigs) || numConfigs == 0)
				{
					if (attribs[19] > 2)
					{
						attribs[19] /= 2;
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
		Win32GLContext(Win32Window* wnd, int multiSamples)
		{
			this->wnd = wnd;
			hDC = GetDC(wnd->GetHWND());

			pf = TryEnableMultisample(multiSamples);

			if (pf == -1)
				SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);

			ctx = wglCreateContext(hDC);
			MakeCurrent();

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			SwapBuffers();

			if (pf != -1) 
				glEnable(GL_MULTISAMPLE_ARB);
		}

		Win32GLContext(Win32GLContext* srcCtx)
		{
			hDC = srcCtx->hDC;
			pf = srcCtx->pf;

			ctx = wglCreateContext(hDC);

			if (!ctx)
				XLI_THROW("Unable to create shared OpenGL context: " + Win32Helpers::GetLastErrorString());

			if (!wglMakeCurrent(hDC, 0))
				XLI_THROW("Unable to make OpenGL context no longer current: " + Win32Helpers::GetLastErrorString());

			if (!wglShareLists(srcCtx->ctx, ctx))
				XLI_THROW("Unable to share OpenGL contexts: " + Win32Helpers::GetLastErrorString());

			srcCtx->MakeCurrent();
		}

		virtual ~Win32GLContext()
		{
			wglDeleteContext(ctx);
		}

		virtual GLContext* CreateSharedContext()
		{
			return new Win32GLContext(this);
		}

		virtual int GetMultiSamples()
		{
			int attr = WGL_SAMPLES_ARB;
			int samples = 0;
			wglGetPixelFormatAttribivARB(hDC, pf, 0, 1, &attr, &samples);
			return samples;
		}

		virtual void SetWindow(Window* window)
		{
			if (window->GetImplementation() != WindowImplementationWin32) XLI_THROW("Unsupported window");
			if (window->GetNativeHandle() == wnd->GetNativeHandle()) return;
			
			this->wnd = (Win32Window*)window;
			this->hDC = GetDC(wnd->GetHWND());
			
			if (pf != -1)
				SetPixelFormat(hDC, pf, &pfd);
			else 
				SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
			
			MakeCurrent();

			if (pf != -1) 
				glEnable(GL_MULTISAMPLE_ARB);
		}

		virtual void MakeCurrent()
		{
			if (!wglMakeCurrent(hDC, ctx))
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
	};

	GLContext* GLContext::Create(Window* window, int multiSamples)
	{
		if (window->GetImplementation() != WindowImplementationWin32) XLI_THROW("Unsupported window");
		return new Win32GLContext((Win32Window*)window, multiSamples);
	}
}
