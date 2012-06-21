
#ifndef __XLI_3_HashSet_H__
#define __XLI_3_HashSet_H__

#include <Xli/Object.h>
#include <Xli/Hash.h>
#include <Xli/Exception.h>

namespace Xli
{
	template <typename T, int bufSize=4> class HashSet: public Object
	{
	private:
		enum BucketState
		{
			BucketStateEmpty = 0,
			BucketStateUsed = 1,
			BucketStateDummy = 2
		};
		struct Bucket
		{
			T Value;
			BucketState State;
		};
		Bucket internalBuckets[bufSize];
		Bucket* buckets;
		int bucketCount;
		int count;

		void rehash(int newSize)
		{
			Bucket* oldBuckets = buckets;
			
			buckets = new Bucket[newSize];
			int oldSize = bucketCount;
			bucketCount = newSize;
			count = 0;

			for (int i = 0; i < bucketCount; i++) buckets[i].State = BucketStateEmpty;

			for (int i = 0; i < oldSize; i++)
			{
				if (oldBuckets[i].State == BucketStateUsed) Add(oldBuckets[i].Value);
			}

			if (oldBuckets != internalBuckets) delete [] oldBuckets;
		}
		

		void expand()
		{
			rehash(bucketCount*2);			
		}

	public:
		HashSet(int initialSizeLog2=0)
		{
			if (initialSizeLog2 == 0)
			{
				bucketCount = bufSize;
				buckets = internalBuckets;
			}
			else
			{
				bucketCount = 1<<initialSizeLog2;
				buckets = new Bucket[bucketCount];
			}
			for (int i = 0; i < bucketCount; i++) buckets[i].State = BucketStateEmpty;
			count = 0;
		}
		~HashSet()
		{
			if (buckets != internalBuckets) delete [] buckets;
		}

		/**
			Gives an integer iterator to the beginning of the map.
		*/
		int Begin() const
		{
			return Next(End());
		}
		
		/**
			Returns the iterator value that indicates the end of the map.
		*/
		int End() const
		{
			return -1;
		}

		/**
			Advances an iterator to the next element.
			@see Begin
			@param iterator The current iterator
			@return An iterator pointing to the succeeding element, or End().
		*/
		int Next(int iterator) const
		{
			for (int i = iterator+1; i < bucketCount; i++)
			{
				if (buckets[i].State == BucketStateUsed) return i;
			}
			return End();
		}

		/**
			Returns the value at the given iterator
			Use Begin(), Next() and End() to iterate through the map.
			@param iterator The iterator at which to look up the value
			@return The value at the given iterator.
		*/
		T& Get(int iterator) const
		{
			if (buckets[iterator].State != BucketStateUsed) XLI_THROW("HashSet::GetValue(): invalid iterator");
			return buckets[iterator].Value;
		}

		void Clear()
		{
			for (int i = 0; i < bucketCount; i++) buckets[i].State = BucketStateEmpty;
			count = 0;
		}

		/**
			Adds a value to the set.
			Returns true if the value was already in the set, false otherwise.
		*/
		bool Add(T value)
		{
			if (count > (bucketCount/8)*5) expand();

			int x = Hash(value) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == BucketStateUsed)
				{
					if (buckets[x].Value == value) return true;
				}
				else if (buckets[x].State == BucketStateEmpty)
				{
					buckets[x].Value = value;
					buckets[x].State = BucketStateUsed;
					count++;
					return false;
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
				if (x == firstX) rehash(bucketCount);
			}
		}

		void Add(const HashSet<T>& set)
		{
			for (int i = set.Begin(); i != set.End(); i = set.Next(i)) Add(set.Get(i));
		}

		void Remove(T value)
		{
			int x = Hash(value) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == BucketStateUsed)
				{
					if (buckets[x].Value == value)
					{
						buckets[x].State = BucketStateDummy;
						count--;
						return;
					}
				}
				else if (buckets[x].State == BucketStateEmpty)
				{
					XLI_THROW("HashSet::Remove(): Map does not contain the given key");
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
				if (x == firstX) rehash(bucketCount);
			}
		}

		bool Contains(T value) const
		{
			int x = Hash(value) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == BucketStateUsed)
				{
					if (buckets[x].Value == value) return true;
				}
				else if (buckets[x].State == BucketStateEmpty)
				{
					return false;
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
				if (x == firstX) { return false; }
			}
		}

		int Count() const
		{
			return count;
		}
	};



}

#endif
