#ifndef __XLI_POOL_H__
#define __XLI_POOL_H__

#include <Xli/Object.h>

namespace Xli
{
	/**
		Pool memory allocator.
		Allocates memory from pages of a custom size. The allocations can not be freed individually.
		Memory is freed when the pool is deleted, invalidating all allocated memory.
		Useful when allocating a large amount of objects with a common lifespan.
	*/
	class Pool: public Object
	{
		struct Page
		{
			UInt32 Size;
			UInt32 Used;
			UInt8 Data[4];
			static Page* New(unsigned int size)
			{
				Page* p = (Page*)new UInt8[size+sizeof(Page)-4];
				p->Used = 0;
				p->Size = size;
				return p;
			}
			static void Delete(Page* p) { delete [] (UInt8*)p; }
			int Capacity() { return Size-Used; }
			void* Alloc(int bytes) { void* ptr = Data+Used; Used += bytes; return ptr; }
		};
		Array<Page*> fullPages;
		Array<Page*> pages;
		int pageSize, fullThreshold;
	public:
		Pool(int pageSize=1024*1024, int fullThreshold=1024): pageSize(pageSize), fullThreshold(fullThreshold)
		{
			pages.Add(Page::New(pageSize));
		}
		virtual ~Pool()
		{
			for (int i = 0; i < fullPages.Length(); i++) Page::Delete(fullPages[i]);
			for (int i = 0; i < pages.Length(); i++) Page::Delete(pages[i]);
		}
		/**
			Allocates a given amount of bytes.
			The allocation is valid until the pool is deleted.
		*/
		void* Alloc(unsigned int bytes)
		{
			// If the allocation is too big for a regular page,
			// allocate dedicated page
			if ((int)bytes > pageSize)
			{
				Page* p = Page::New(bytes);
				fullPages.Add(p);
				return p->Alloc(bytes);
			}

			// Try to allocate on an existing page
			for (int i = 0; i < pages.Length(); i++)
			{
				if ((int)bytes < pages[i]->Capacity())
				{
					void* ptr = pages[i]->Alloc(bytes);
					if (pages[i]->Capacity() < fullThreshold)
					{
						fullPages.Add(pages[i]);
						pages.RemoveAt(i);
					}
					return ptr;
				}
			}

			// No space on existing pages, allocate new
			Page* p = Page::New(pageSize);
			pages.Add(p);
			return p->Alloc(bytes);
		}
	};
}

#endif
