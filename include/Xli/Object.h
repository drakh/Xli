#ifndef __XLI_OBJECT_H__
#define __XLI_OBJECT_H__

namespace Xli
{
	class String;

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
		
        virtual String ToString() const;
	};
}


#endif
