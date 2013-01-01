#ifndef __XLI_MANAGED_H__
#define __XLI_MANAGED_H__

#include <Xli/Config.h>
#include <Xli/Exception.h>

namespace Xli
{
	/**
		\ingroup Containers
	*/
	template <class T> class Managed
	{
	protected:
		T* object;

	public:
		Managed()
		{
			this->object = 0;
		}
		Managed(T* object)
		{
			this->object = object;
		}
		Managed(const Managed& managed)
		{
			this->object = managed.object;
			if (object) object->AddRef();
		}
		~Managed()
		{
			if (object) object->Release();
		}
		bool IsNull() const
		{
			return !object;
		}
		bool IsSet() const
		{
			return object != 0;
		}
		Managed& operator = (T* ptr)
		{
			if (object) object->Release();
			object = ptr;
			return *this;
		}
		Managed& operator = (const Managed& managed)
		{
			if (object) object->Release();
			object = managed.object;
			if (object) object->AddRef();
			return *this;
		}
		T* Get()
		{
#ifdef XLI_NULL_POINTER_CHECK
			if (IsNull()) XLI_THROW_NULL_POINTER;
#endif
			return object;
		}
		const T* Get() const
		{
#ifdef XLI_NULL_POINTER_CHECK
			if (IsNull()) XLI_THROW_NULL_POINTER;
#endif
			return object;
		}
		T* Pointer()
		{
			return object;
		}
		const T* Pointer() const
		{
			return object;
		}
		operator T*()
		{
			return Pointer();
		}
		operator const T*() const
		{
			return Pointer();
		}
		T* operator ->()
		{
			return Get();
		}
		const T* operator ->() const
		{
			return Get();
		}
	};

	/**
		\ingroup Containers
	*/
	template <class T> class Shared: public Managed<T>
	{
	public:
		Shared()
		{
		}
		Shared(T* ptr): Managed<T>(ptr)
		{
			if (this->object) this->object->AddRef();
		}
		Shared(const Shared& shared): Managed<T>(shared.object)
		{
			if (this->object) this->object->AddRef();
		}
		Shared(const Managed<T>& managed): Managed<T>(managed)
		{
		}
		~Shared()
		{
		}
		Shared& operator = (T* ptr)
		{
			if (ptr) ptr->AddRef();
			if (this->object) this->object->Release();
			this->object = ptr;
			return *this;
		}
		Shared& operator = (const Managed<T>& managed)
		{
			T* ptr = const_cast<T*>(managed.Pointer());
			if (ptr) ptr->AddRef();
			if (this->object) this->object->Release();
			this->object = ptr;
			return *this;
		}
		Shared& operator = (const Shared& shared)
		{
			if (shared.object) shared.object->AddRef();
			if (this->object) this->object->Release();
			this->object = shared.object;
			return *this;
		}
		operator T*()
		{
			return this->object;
		}
		operator const T*() const
		{
			return this->object;
		}
	};

	/**
		\ingroup Containers
	*/
	template <class T> Managed<T> Manage(T* oject)
	{
		return oject;
	}

	/**
		\ingroup Containers
	*/
	template <class T> T* Share(T* object)
	{
		if (object) object->AddRef();
		return object;
	}
}


#endif
