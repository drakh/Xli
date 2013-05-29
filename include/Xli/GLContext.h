#ifndef __XLI_GL_CONTEXT_H__
#define __XLI_GL_CONTEXT_H__

#include <Xli/Window.h>

namespace Xli
{
	/**
		\ingroup WM
	 */
	class GLContext: public Object
	{
	public:
		static GLContext* Create(Window* window, int multiSamples);

		virtual ~GLContext() {}

		virtual GLContext* CreateSharedContext() = 0;

		virtual void SetWindow(Window* window) { XLI_THROW_NOT_SUPPORTED(XLI_FUNC); }

        virtual void MakeCurrent() = 0;
		virtual void SwapBuffers() = 0;

		/**
			Sets the swap interval.
			@param interval 1 enables VSync, 0 disables VSync
			@return true on success, false on failure
		*/
		virtual bool SetSwapInterval(int interval) = 0;

		virtual int GetSwapInterval() = 0;

		virtual int GetMultiSamples() = 0;

		virtual unsigned int GetBackbufferHandle() = 0;
	};
}

#endif
