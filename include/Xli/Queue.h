#ifndef __XLI_QUEUE_H__
#define __XLI_QUEUE_H__

#include "Array.h"

namespace Xli
{
	template <typename T> class Queue: public Object
	{
		struct Node
		{
			Node* Next;
			T Value;
		};

		Node* head;
		Node* tail;

		Array<Node*> freeNodes;
		int len;

	public:
		Queue()
		{
			head = 0;
			tail = 0;
			len = 0;
		}
		virtual ~Queue()
		{
			Clear();
			Trim();
		}

		void Trim()
		{
			for (int i = 0; i < freeNodes.Length(); i++)
			{
				delete freeNodes[i];
			}

			freeNodes.Clear();
			freeNodes.Trim();
		}

		void Clear()
		{
			while (head)
			{
				Node* temp = head;
				head = head->Next;
				freeNodes.Add(temp);
			}

			tail = 0;
			len = 0;
		}

		void Enqueue(const T& value)
		{
			Node* n;
			
			if (freeNodes.Length()) n = freeNodes.RemoveLast();
			else n = new Node();

			n->Next = 0;
			n->Value = value;

			if (tail)
			{
				tail->Next = n;
				tail = n;
			}
			else
			{
				head = n;
				tail = n;
			}

			len++;
		}

		const T& Peek() const
		{
#ifdef XLI_RANGE_CHECK
			if (!head) XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
			return head->Value;
		}

		T Dequeue()
		{
#ifdef XLI_RANGE_CHECK
			if (!head) XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
			Node* oldHead = head;
			head = head->Next;
			if (!head) tail = 0;
			T value = oldHead->Value;
			freeNodes.Add(oldHead);
			len--;
			return value;
		}

		int Count() const
		{
			return len;
		}

		bool IsEmpty() const
		{
			return len == 0;
		}
	};
}


#endif
