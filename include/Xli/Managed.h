#ifndef __XLI_MANAGED_H__
#define __XLI_MANAGED_H__

#include <Xli/Config.h>
#include <Xli/Exception.h>

namespace Xli
{
	template <class T> class Managed;

	template <class T> class Local
	{
	protected:
		T* object;

	public:
		Local(T* object)
		{
			if (!object) XLI_THROW_NULL_POINTER;
			this->object = object;
		}
		Local(const Local& local)
		{
			this->object = local.object;
			this->object->AddRef();
		}

		Local(const Managed<T>& managed);

		~Local()
		{
			object->Release();
		}
		const T* Pointer() const
		{
			return object;
		}
		T* Pointer()
		{
			return object;
		}
		const T* Get() const
		{
			return object;
		}
		T* Get()
		{
			return object;
		}
		operator T*()
		{
			return object;
		}
		operator const T*() const
		{
			return object;
		}
		T* operator ->()
		{
			return object;
		}
		const T* operator ->() const
		{
			return object;
		}
	};

	template <class T> class Valid: public Local<T>
	{
	public:
		Valid(T* object): Local<T>(object)
		{
			this->object->AddRef();
		}
		Valid(const Valid& valid): Local<T>(valid)
		{
			this->object->AddRef();
		}

		Valid(const Managed<T>& managed);

		~Valid()
		{
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
		Managed(const Local<T>& local)
		{
			this->object = const_cast<T*>(local.Get());
			this->object->AddRef();
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
		Shared(const Local<T>& local): Managed<T>(local)
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

	template <typename T> Local<T>::Local(const Managed<T>& managed)
	{
		if (managed.IsNull()) XLI_THROW_NULL_POINTER;
		this->object = const_cast<T*>(managed.Pointer());
		this->object->AddRef();
	}

	template <typename T> Valid<T>::Valid(const Managed<T>& managed): Local<T>(managed)
	{
	}

	template <class T> Managed<T> Manage(T* oject)
	{
		return oject;
	}

	template <class T> T* Share(T* object)
	{
		if (object) object->AddRef();
		return object;
	}

	template <typename T> T* Assert(T* object)
	{
		if (!object) XLI_THROW_NULL_POINTER;
		return object;
	}
}


#endif
