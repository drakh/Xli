#ifndef __XLI_STACK_H__
#define __XLI_STACK_H__

#include <Xli/Array.h>

namespace Xli
{
	template <typename T, int bufSize=4> class Stack: public Object
	{
		Array<T, bufSize> data;

	public:
		void Reserve(unsigned int newCapacity)
		{
			data.Reserve(newCapacity);
		}
		
		void Resize(unsigned int newSize)
		{
			data.Reserve(newSize);
		}
		
		int Count() const
		{
			return data.Length();
		}
		
		int Push(T item)
		{
			return data.Add(item);
		}

		int Push(const Stack<T>& stack)
		{
			return data.Add(stack.data);
		}

		bool IsEmpty() const
		{
			return data.Length() == 0;
		}

		T& operator [] (int index)
		{
			return data[index];
		}

		T& Peek()
		{
			return data.Last();
		}

		T Pop()
		{
			return data.RemoveLast();
		}

		T Pop(int count)
		{
			data.RemoveLast(count);
		}

		const T& Get(int index) const
		{
			return data.Get(index);
		}

		void Set(int index, const T& value)
		{
			return data.Set(index, value);
		}
		
		void Clear()
		{
			data.Clear();
		}

		bool Contains(const T& item)
		{
			return data.Contains(item);
		}

		int IndexOf(const T& item)
		{
			return data.IndexOf(item);
		}
	};
}


#endif
