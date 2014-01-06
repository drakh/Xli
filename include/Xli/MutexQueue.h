#ifndef __XLI_MUTEXQUEUE_H__
#define __XLI_MUTEXQUEUE_H__

#include <Xli/Mutex.h>
#include "Array.h"

namespace Xli
{
    /**
        \ingroup XliCoreContainers
    */
    template <typename T> class MutexQueue: public Object
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

        MutexHandle mutex;

    public:
        MutexQueue()
        {
            mutex = CreateMutex();
            head = 0;
            tail = 0;
            len = 0;
        }

        virtual ~MutexQueue()
        {
            Clear();
            Trim();
        }

        void Trim()
        {
            LockMutex(mutex);
            for (int i = 0; i < freeNodes.Length(); i++)
                delete freeNodes[i];

            freeNodes.Clear();
            freeNodes.Trim();
            UnlockMutex(mutex);
        }

        void Clear()
        {
            LockMutex(mutex);
            while (head)
            {
                Node* temp = head;
                head = head->Next;
                freeNodes.Add(temp);
            }

            tail = 0;
            len = 0;
            UnlockMutex(mutex);
        }

        void Enqueue(const T& value)
        {
            LockMutex(mutex);
            Node* n = freeNodes.Length() ? freeNodes.RemoveLast() : new Node();

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
            UnlockMutex(mutex);
        }

        T Dequeue()
        {
            LockMutex(mutex);
#ifdef XLI_RANGE_CHECK
            if (!head) XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
            Node* oldHead = head;
            head = head->Next;
            if (!head) tail = 0;
            T value = oldHead->Value;
            freeNodes.Add(oldHead);
            len--;
            UnlockMutex(mutex);
            return value;
        }

        int Count() const
        {
            LockMutex(mutex);
            int length = len;
            UnlockMutex(mutex);
            return len;
        }

        bool IsEmpty() const
        {
            LockMutex(mutex);
            bool empty = len == 0;
            UnlockMutex(mutex);
            return empty;
        }
    };
}


#endif
