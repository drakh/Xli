#ifndef __XLI_ARRAY_H__
#define __XLI_ARRAY_H__

#include <Xli/Config.h>
#include <Xli/Object.h>
#include "Sort.h"

namespace Xli
{
	template <typename T, uint TBufSize> class Array: public Object
	{
		T* data;
		T buf[TBufSize];
		int used;
		int capacity;

	public:
		Array()
		{
			used = 0;
			capacity = TBufSize;
			data = buf;
		}
		Array(int size)
		{
			used = 0;
			capacity = TBufSize;
			data = buf;
			Resize(size);
		}
		Array(int count, const T* initItems)
		{
			used = 0;
			capacity = TBufSize;
			data = buf;
			Resize(count);
			for (int i = 0; i < count; i++) data[i] = initItems[i];
		}
		~Array()
		{
			if (data != buf) delete [] data;
		}
		Array(const Array& a)
		{
			used = 0;
			capacity = TBufSize;
			data = buf;
			Resize(a.Length());

			for (int i = 0; i < Length(); i++)
			{
				data[i] = a.data[i];
			}
		}
		Array& operator = (Array& a)
		{
			Resize(a.Length());

			for (int i = 0; i < Length(); i++)
			{
				data[i] = a.data[i];
			}

			return *this;
		}

		void Reserve(int newCapacity)
		{
			if (newCapacity > capacity)
			{
				T* newData = new T[newCapacity];
				for (int i = 0; i < used; i++) newData[i] = data[i];
				if (data != buf) delete [] data;
				data = newData;
				capacity = newCapacity;
			}
		}

		void Trim()
		{
			if (data != buf)
			{
				if (used <= (int)TBufSize)
				{
					for (int i = 0; i < used; i++)
					{
						buf[i] = data[i];
					}
					delete [] data;
					data = buf;
					capacity = TBufSize;
				}
				else if (used < capacity)
				{
					T* newData = new T[used];
					for (int i = 0; i < used; i++)
					{
						newData[i] = data[i];
					}
					delete [] data;
					data = newData;
					capacity = used;
				}
			}
		}

		void Resize(int newSize)
		{
			Reserve(newSize);
			used = newSize;
		}

		void Expand()
		{
			if (used > capacity/2) Reserve(capacity*2);
		}

		int Length() const
		{
			return used;
		}

		int FirstIndex() const
		{
			return 0;
		}

		int LastIndex() const
		{
			return used - 1;
		}

		int Add()
		{
			if (capacity <= used) Reserve(capacity*2);
			return used++;
		}

		int Add(const T& item)
		{
			if (capacity <= used) Reserve(capacity*2);
			data[used] = item;
			return used++;
		}

		int Add(const T* items, int count)
		{
			int res = used;
			for (int i = 0; i < count; i++) Add(items[i]);
			return res;
		}

		int Add(const Array<T>& values)
		{
			return Add(values.data, values.used);
		}

		int Add(int index, const T& item)
		{
#ifdef XLI_RANGE_CHECK
			if (index > used || index < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			Add(item);
			for (int i = used-1; i > index; i--) data[i] = data[i-1];
			data[index] = item;
			return index;
		}

		int IndexOf(const T& value) const
		{
			for (int i = 0; i < used; i++) if (data[i] == value) return i;
			return -1;
		}

		bool Contains(const T& value) const
		{
			int i = IndexOf(value);
			return i != -1;
		}

		void RemoveAt(int index)
		{
#ifdef XLI_RANGE_CHECK
			if (index >= used || index < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			for (int i = index; i < used-1; i++) data[i] = data[i+1];
			used--;
		}

		bool Remove(const T& item)
		{
			int i = IndexOf(item);
			if (i != -1)
			{
				RemoveAt(i);
				return true;
			}
			return false;
		}

		void RemoveAll(const T& item)
		{
			while (Remove(item));
		}

		void SwapRemoveAt(int i)
		{
			Swap(Get(i), Last());
			used--;
		}

		void RemoveRange(int start, int count)
		{
#ifdef XLI_RANGE_CHECK
			if (start >= used || start < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
			if (start+count > used || count < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			for (int i = start; i < used-count; i++) data[i] = data[i+count];
			used -= count;
		}

		T RemoveFirst()
		{
			T temp = First();
			RemoveAt(0);
			return temp;
		}

		T RemoveLast()
		{
			T temp = Last();
			RemoveAt(used-1);
			return temp;
		}

		int FirstIndexOf(const T& elm) const
		{
			return IndexOf(elm);
		}

		int LastIndexOf(const T& elm) const
		{
			for (int i = used-1; i >= 0; i--) if (data[i] == elm) return i;
			return -1;
		}

		void RemoveLastInstanceOf(const T& elm)
		{
			int i = LastIndexOf(elm);
			RemoveAt(i);
		}

		void RemoveFirstInstanceOf(const T& elm)
		{
			int i = FirstIndexOf(elm);
			RemoveAt(i);
		}

		void SwapRemoveLastInstanceOf(const T& elm)
		{
			int i = LastIndexOf(elm);
			SwapRemoveAt(i);
		}

		void SwapRemoveFirstInstanceOf(const T& elm)
		{
			int i = FirstIndexOf(elm);
			SwapRemoveAt(i);
		}

		/**
			Removes the first given number of elements.
			@return The first element in the range.
		*/
		T RemoveFirst(int count)
		{
			RemoveRange(1, count-1);
			return RemoveAt(0);
		}

		/**
			Removes the last given number of elements.
			@return The last element in the range.
		*/
		T RemoveLast(int count)
		{
			RemoveRange(used-count, count-1);
			return RemoveLast();
		}

		T& Get(int index)
		{
#ifdef XLI_RANGE_CHECK
			if (index >= used || index < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			return data[index];
		}

		const T& Get(int index) const
		{
#ifdef XLI_RANGE_CHECK
			if (index >= used || index < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			return data[index];
		}

		void Set(int index, const T& elm)
		{
#ifdef XLI_RANGE_CHECK
			if (index >= used || index < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			data[index] = elm;
		}

		void Clear()
		{
			used = 0;
		}

		T& operator [] (int index)
		{
			return Get(index);
		}
		const T& operator [] (int index) const
		{
			return Get(index);
		}

		T* Data()
		{
			return data;
		}
		const T* Data() const
		{
			return data;
		}

		T& First()
		{
			return Get(0);
		}
		const T& First() const
		{
			return Get(0);
		}

		T& Last()
		{
			return Get(Length()-1);
		}
		const T& Last() const
		{
			return Get(Length()-1);
		}

		void Reverse()
		{
			for (int a = 0, b = used; a < --b; a++)
			{
				Swap(data[a], data[b]);
			}
		}

		template <typename TComparator> void Sort()
		{
			Xli::Sort<T, TComparator>(data, 0, used);
		}

    private:
        template <typename S> struct RS
        {
            S SValue;
            T TValue;
            operator const S&() const { return SValue; }
        };
        
    public:
		/**
			Sort this array based on the reordering given by sorting another array.
			@param s The array which defines the reordering of this array (will also be sorted).
		*/
		template <typename S, typename SComparator> void MultiSort(Array<S>& s)
		{
			if (Length() != s.Length()) XLI_THROW("Arrays must have the same length");

			Array<RS<S> > rs(used);
			for (int i = 0; i < rs.Length(); i++)
			{
				rs[i].SValue = s.Get(i);
				rs[i].TValue = Get(i);
			}
			rs.Sort<SComparator>();
			for (int i = 0; i < rs.Length(); i++)
			{
				s[i] = rs[i].SValue;
				data[i] = rs[i].TValue;
			}
		}

		template <typename S> void MultiSort(Array<S>& s)
		{
			MultiSort<S, ComparatorLessThan<S> >(s);
		}

		void Sort()
		{
			Sort<ComparatorLessThan<T> >();
		}
		void SortReversed()
		{
			Sort<ComparatorGreaterThan<T> >();
		}
		void SortPointers()
		{
			Sort<ComparatorPointerLessThan<T> >();
		}
		void SortPointersReversed()
		{
			Sort<ComparatorPointerGreaterThan<T> >();
		}

	};
}


#endif
