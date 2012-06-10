#ifndef __XLI_GL_CONTEXT_H__
#define __XLI_GL_CONTEXT_H__

#include "GLCommon.h"
#include <XliPlatform/Window.h>

namespace Xli
{
	class GLContext: public Object
	{
	public:
		static GLContext* Create(Window* window, int multiSamples);

		virtual ~GLContext() {}

		virtual GLContext* CreateSharedContext() = 0;

		virtual void SetWindow(Window* window) { XLI_THROW_NOT_SUPPORTED(__FUNCTION__); }

        virtual void MakeCurrent() = 0;
		virtual void SwapBuffers() = 0;

		/**
			Sets the swap interval.
			@param interval 1 enables VSync, 0 disables VSync
			@return true on success, false on failure
		*/
		virtual bool SetSwapInterval(int interval) = 0;

		// Broken
		//virtual void SetMultiSamples(int value) = 0;
		
		virtual int GetMultiSamples() = 0;
	};
}

#endif
