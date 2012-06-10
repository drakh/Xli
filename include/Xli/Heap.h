#ifndef __XLI_HEAP_H__
#define __XLI_HEAP_H__

#include "Config.h"
#include "Array.h"
#include "Utils.h"

namespace Xli
{
	template <typename T, typename TComparator> class Heap: public Object
	{
		Array<T> data;

	public:
		static void SiftDown(Array<T>& data, int first, int last)
		{
			while (true)
			{
				int largestChild = (first << 1) + 1;

				if (largestChild <= last)
				{
					if (largestChild + 1 <= last)
					{
						if (TComparator::Compare(data[largestChild], data[largestChild + 1]))
						{
							largestChild++;
						}
					}
					if (TComparator::Compare(data[first], data[largestChild]))
					{
						Swap(data[first], data[largestChild]);
						first = largestChild;
						continue;
					}
				}

				break;
			}
		}

		static void SiftUp(Array<T>& data, int first, int last)
		{
			while (last > first)
			{
				int parent = (last - 1) >> 1;
				if (TComparator::Compare(data[parent], data[last]))
				{
					Swap(data[last], data[parent]);
					last = parent;
				}
				else
				{
					return;
				}
			}
		}

		static void Heapify(Array<T>& data)
		{
			int first = (data.Length() - 2) >> 1;
			int last = data.Length() - 1;

			while (first >= 0)
			{
				SiftDown(data, first, last);
				first--;
			}
		}

		Heap()
		{
		}
		Heap(const Array<T>& values)
		{
			data = values;
			Heapify(data);
		}

		Heap(int capacity)
		{
			data.Reserve(capacity);
		}
		void Reserve(int capacity)
		{
			data.Reserve(capacity);
		}
		bool IsEmpty()
		{
			return data.Length() == 0;
		}
		void Push(const T& item)
		{
			int i = data.Add(item);
			SiftUp(data, 0, i);
		}
		T Pop()
		{
#ifdef XLI_RANGE_CHECK
			if (data.Length() == 0) XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
			Swap(data.First(), data.Last());
			SiftDown(data, 0, data.Length() - 2);
			return data.RemoveLast();
		}
		const T& Peek()
		{
#ifdef XLI_RANGE_CHECK
			if (data.Length() == 0) XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
			return data[0];
		}
		int Size()
		{
			return data.Length();
		}
		const T& Get(int index)
		{
			return data[index];
		}
		void Clear()
		{
			data.Clear();
		}
	};

	template <typename T, typename TComparator> void HeapSort(Array<T>& data)
	{
		Heap<T, TComparator>::Heapify(data);

		int last = data.Length() - 1;
		while (last > 0)
		{
			Swap(data[last], data[0]);
			last--;
			Heap<T, TComparator>::SiftDown(data, 0, last);
		}
	}

	template <typename T> class MinHeap: public Heap<T, ComparatorGreaterThan<T> > {};
	template <typename T> class MaxHeap: public Heap<T, ComparatorLessThan<T> > {};
}

#endif
