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
		Object() 
		{ 
			refCount = 1; 
		}

		virtual ~Object() 
		{
			if (refCount > 1)
			{
				XLI_THROW_BAD_DELETE;
			}
		}

		int GetRefCount() const
		{
			return refCount;
		}

		void AddRef() 
		{
			refCount++;
		}

		void Release()
		{
			refCount--;			
			if (refCount) return;
			Delete();
		}

		virtual void Delete() 
		{ 
			delete this; 
		}

		virtual String ToString()
		{
			return "<Object " + Xli::ToString((void*)this) + ">";
		}
	};
}


#endif
