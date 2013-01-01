#ifndef __XLI_PROGRESS_CALLBACK_H__
#define __XLI_PROGRESS_CALLBACK_H__

#include <Xli/Object.h>

namespace Xli
{
	/**
		\ingroup Media
	*/
	class ProgressCallback: public Object
	{
	public:
		virtual void Update(double current, double total) = 0;
	};
}

#endif
