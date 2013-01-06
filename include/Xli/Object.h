#ifndef __XLI_OBJECT_H__
#define __XLI_OBJECT_H__

#include <Xli/Exception.h>

namespace Xli
{
	String ToString(void* p);

	class Object
	{
		int refCount;

	public:
		Object();
		virtual ~Object();

		int GetRefCount() const;

		void AddRef();
		void Release();

		virtual void Delete();
		virtual String ToString();
	};
}


#endif
