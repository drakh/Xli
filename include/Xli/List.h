#ifndef __XLI_LIST_H__
#define __XLI_LIST_H__

#include <Xli/Array.h>
#include <Xli/Managed.h>

namespace Xli
{
	/**
		Wraps an Array in a pass-by-reference-type.
	*/
	template <typename T> class List
	{
		Array<T>* data;

	public:
		List()
		{
			data = 0;
		}

		List(int size)
		{
			data = new Array<T>(size);
		}
		
		List(const List& list)
		{
			data = list.data;
			if (data) data->AddRef();
		}

		List(Array<T>* ar)
		{
			if (ar) ar->AddRef();
			data = ar;
		}

		static List Create(T firstItem)
		{
			List list;
			list.Add(firstItem);
			return list;
		}

		inline List& operator = (const List& object)
		{
			if (data) data->Release();
			this->data = object.data;
			if (data) data->AddRef();
			return *this;
		}

		bool operator == (List& list)
		{
			if (Length() != list.Length()) return false;
			if (!Length()) return true;
			for (int i = 0; i < Length(); i++) if (data->Get(i) != list[i]) return false;
			return true;
		}

		bool operator != (List& list)
		{
			return !((*this) == list);
		}
		
		/**
			Allows construction from list of another type.
			This requires T to have a constructor that takes CT as argument.
		*/
		template <typename CT> List(List<CT>& list)
		{
			data = new Array<T>();
			for (int i = 0; i < list.Length(); i++) data->Add(T(list[i]));
		}
		
		~List()
		{
			if (data) data->Release();
		}

		Xli::Array<T>& GetArray()
		{
			if (!data) data = new Array<T>();
			return *data;
		}

		List<T> Clone() const
		{
			if (!data) return List<T>();
			List<T> clone;
			clone.data = new Array<T>(*data);
			return clone;
		}
		
		int Length() const
		{
			if (!data) return 0;
			return data->Length();
		}

		int IndexOf(const T& item) const
		{
			if (!data) return -1;
			return data->IndexOf(item);
		}

		int LastIndexOf(const T& item) const
		{
			if (!data) return -1;
			return data->LastIndexOf(item);
		}

		void Resize(int newSize)
		{
			if (!data) data = new Array<T>();
			data->Resize(newSize);
		}
		
		int Add(const T& item)
		{
			if (!data) data = new Array<T>();
			return data->Add(item);
		}

		int Add(int index, const T& item)
		{
			if (!data) data = new Array<T>();
			return data->Add(index, item);
		}

		int Add(const T* items, int count)
		{
			if (!data) data = new Array<T>();
			return data->Add(items, count);
		}

		int Add(const List<T>& list)
		{
			if (!list.data) return -1;
			if (!data) data = new Array<T>();
			return data->Add(*list.data);
		}

		void Remove(const T& item)
		{
			if (!data) XLI_THROW("List does not contain the given element");
			data->Remove(item);
		}
			
		void Clear()
		{
			if (data) data->Clear();
		}

		T& operator [] (int index)
		{
			if (!data) 
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
			return data->Get(index);
		}
		const T& operator [] (int index) const
		{
			if (!data) 
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
			return data->Get(index);
		}

		T& First()
		{
			return data->First();
		}
		const T& First() const
		{
			return data->First();
		}

		T& Last()
		{
			return data->Last();
		}
		const T& Last() const
		{
			return data->Last();
		}

		T* Data()
		{
			if (!data) return 0;
			return data->Data();
		}
		const T* Data() const
		{
			if (!data) return 0;
			return data->Data();
		}

		bool Contains(const T& value) const
		{
			if (!data) return false;
			return data->Contains(value);
		}

		T RemoveLast()
		{
			if (!data) XLI_THROW_INDEX_OUT_OF_BOUNDS;
			return data->RemoveLast();
		}

		T RemoveFirst()
		{
			if (!data) XLI_THROW_INDEX_OUT_OF_BOUNDS;
			return data->RemoveFirst();
		}

		void RemoveAt(int index)
		{
			if (!data) XLI_THROW_INDEX_OUT_OF_BOUNDS;
			data->RemoveAt(index);
		}

		template <typename TComparator> void Sort()
		{
			if (data) data->Sort<TComparator>();
		}

		template <typename S, typename TComparator> void MultiSort(Array<S>& s)
		{
			if (data) data->MultiSort<TComparator>(s);
			else if (s.Length()) XLI_THROW("Lists must have the same length");
		}

		template <typename S> void MultiSort(Array<S>& s)
		{
			if (data) data->MultiSort<S>(s);
			else if (s.Length()) XLI_THROW("Lists must have the same length");
		}

		template <typename S, typename TComparator> void MultiSort(List<S>& s)
		{
			MultiSort<TComparator>(s.GetArray());
		}

		template <typename S> void MultiSort(List<S>& s)
		{
			MultiSort(s.GetArray());
		}

		void Sort()
		{
			if (data) data->Sort();
		}
		void SortReversed()
		{
			if (data) data->SortReversed();
		}
		void SortPointers()
		{
			if (data) data->SortPointers();
		}
		void SortPointersReversed()
		{
			if (data) data->SortPointersReversed();
		}
	};

	template <typename T> class ManagedList: public List<Managed<T> >
	{
	public:
		void Remove(T* item)
		{
			Shared<T> bitem(item);
			List<Managed<T> >::Remove(bitem);
		}
		int IndexOf(T* item)
		{
			Shared<T> bitem(item);
			return List<Managed<T> >::IndexOf(bitem);
		}
	};
}


#endif
