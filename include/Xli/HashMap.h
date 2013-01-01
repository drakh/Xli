#ifndef __XLI_HASH_MAP_H__
#define __XLI_HASH_MAP_H__

#include <Xli/Array.h>
#include <Xli/Hash.h>

namespace Xli
{
	/**
		\ingroup Containers
	*/
	enum HashBucketState
	{
		HashBucketStateEmpty = 0,
		HashBucketStateUsed = 1,
		HashBucketStateDummy = 2
	};

	/**
		\ingroup Containers
	*/
	template <typename TKey, typename TValue> struct HashBucket
	{
		TKey Key;
		TValue Value;
		HashBucketState State;
	};

	/**
		\ingroup Containers
	*/
	template <typename TKey, typename TValue> class HashMapDefaultTraits
	{
	public:
		inline static UInt32 Hash(const TKey& key) { return Xli::Hash(key); }
		inline static bool Equals(const TKey& a, const TKey& b) { return a == b; }
		inline static HashBucket<TKey, TValue>* NewBuckets(int buckets, void* memPool) { return new HashBucket<TKey, TValue>[buckets]; }
		inline static void DeleteBuckets(HashBucket<TKey, TValue>* ptr, void* memPool) { delete [] ptr; }
	};

	/**
		\ingroup Containers
	*/
	template <typename TKey, typename TValue> class HashMapConstCastTraits
	{
	public:
		inline static UInt32 Hash(const TKey& key) { return Xli::Hash(const_cast<TKey>(key)); }
		inline static bool Equals(const TKey& a, const TKey& b) { return const_cast<TKey>(a) == const_cast<TKey>(b); }
		inline static HashBucket<TKey, TValue>* NewBuckets(int buckets, void* memPool) { return new HashBucket<TKey, TValue>[buckets]; }
		inline static void DeleteBuckets(HashBucket<TKey, TValue>* ptr, void* memPool) { delete [] ptr; }
	};

	/**
		\ingroup Containers

		Dictionary template.
		Maps keys to values using a very efficient hash map. For user-defined TKey types, methods
		UInt32 Hash() and operator == must be defined. All built-in C++ types can be used directly
		as TKey. If the template argument TTraits can be specified to override the Hash() and Equals()
		functions used. This is useful i.e. when using pointers to objects as keys and you want to
		compare the value of the objects and not the pointers.
	*/
	template <typename TKey, typename TValue, typename TTraits = HashMapDefaultTraits<TKey, TValue>, int bufSize=8> class HashMap: public Object
	{
	private:
		typedef HashBucket<TKey, TValue> Bucket;
		Bucket internalBuckets[bufSize];
		Bucket* buckets;
		void* memPool;

		int bucketCount;
		int count;

		void rehash(int newSize)
		{
			Bucket* oldBuckets = buckets;

			buckets = TTraits::NewBuckets(newSize, memPool);
			int oldSize = bucketCount;
			bucketCount = newSize;
			count = 0;

			for (int i = 0; i < bucketCount; i++) buckets[i].State = HashBucketStateEmpty;

			for (int i = 0; i < oldSize; i++)
			{
				if (oldBuckets[i].State == HashBucketStateUsed) (*this)[oldBuckets[i].Key] = oldBuckets[i].Value;
			}

			if (oldBuckets != internalBuckets) TTraits::DeleteBuckets(oldBuckets, memPool);
		}


		void expand()
		{
			rehash(bucketCount*2);
		}

	public:
		explicit HashMap(int initialSizeLog2 = 0, void* memPool = 0)
		{
			this->memPool = memPool;

			if (initialSizeLog2 == 0)
			{
				bucketCount = bufSize;
				buckets = internalBuckets;
			}
			else
			{
				bucketCount = 1<<initialSizeLog2;
				buckets = TTraits::NewBuckets(bucketCount, memPool);
			}
			for (int i = 0; i < bucketCount; i++) buckets[i].State = HashBucketStateEmpty;
			count = 0;
		}
		~HashMap()
		{
			if (buckets != internalBuckets)
			{
				TTraits::DeleteBuckets(buckets, memPool);
			}
		}

		HashMap& operator = (const HashMap& map)
		{
			Clear();
			for (int it = map.Begin(); it != map.End(); it = map.Next(it))
			{
				Add(map.GetKey(it), map.GetValue(it));
			}
			return *this;
		}

		Bucket* GetBucketBuffer() { return buckets; }
		void SetBucketBuffer(Bucket* buf) { TTraits::DeleteBuckets(buckets, memPool); buckets = buf; }

		/**
			Returns the number of hash buckets currently in use in the map.
		*/
		int GetBucketCount() const
		{
			return bucketCount;
		}

		/**
			Returns a hash bucket with a given index.
		*/
		const Bucket& GetBucket(int index) const
		{
			return buckets[index];
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
				if (buckets[i].State == HashBucketStateUsed) return i;
			}
			return End();
		}

		/**
			Returns the value at the given iterator
			Use Begin(), Next() and End() to iterate through the map.
			@param iterator The iterator at which to look up the value
			@return The value at the given iterator.
		*/
		const TValue& GetValue(int iterator) const
		{
			if (buckets[iterator].State != HashBucketStateUsed) XLI_BREAK_THROW("Invalid iterator");
			return buckets[iterator].Value;
		}

		/**
			Returns the value at the given iterator
			Use Begin(), Next() and End() to iterate through the map.
			@param iterator The iterator at which to look up the value
			@return The value at the given iterator.
		*/
		TValue& GetValue(int iterator)
		{
			if (buckets[iterator].State != HashBucketStateUsed) XLI_BREAK_THROW("Invalid iterator");
			return buckets[iterator].Value;
		}

		/**
			Sets a new value at the given iterator
			Use Begin(), Next() and End() to iterate through the map.
			@param iterator The iterator at which to look up the value
			@param value The new value to be set at the given iterator.
		*/
		void SetValue(int iterator, const TValue& value)
		{
			if (buckets[iterator].State != HashBucketStateUsed) XLI_BREAK_THROW("Invalid iterator");
			buckets[iterator].Value = value;
		}

		/**
			Returns the key at the given iterator
			Use Begin(), Next() and End() to iterate through the map.
			@param iterator The iterator at which to look up the key
			@return The key at the given iterator.
		*/
		const TKey& GetKey(int iterator) const
		{
			if (buckets[iterator].State != HashBucketStateUsed) XLI_BREAK_THROW("Invalid iterator");
			return buckets[iterator].Key;
		}

		/**
			Returns a key with the given value.
			If more than one key-value-pair contains the same value, which of the keys this funciton returns is undefined.
			To get all keys with a given value, use GetKeysFromValue().
			This is a very slow lookup, as it requires a linear iteration through the hash map.
			If you want to perform this operation frequently, consider using a BiHashMap.
			@param value The value to look up
			@return The key at the given value.
		*/
		const TKey& GetKeyFromValue(const TValue& value) const
		{
			for (int i = Begin(); i != End(); i = Next(i))
			{
				if (GetValue(i) == value) return GetKey(i);
			}
			XLI_BREAK_THROW("Value not found");
		}

		bool ContainsValue(const TValue& value) const
		{
			for (int i = Begin(); i != End(); i = Next(i))
			{
				if (GetValue(i) == value) return true;
			}
			return false;
		}

		/**
			Enumerates all keys with the given value.
			This is a very slow lookup, as it requires a linear iteration through the hash map.
			If you want to perform this operation frequently, consider using a BiHashMap.
			@param value The value to look up
			@param keys An array to fill with the found keys.
		*/
		void GetKeysFromValue(const TValue& value, Array<TKey>& keys) const
		{
			for (int i = Begin(); i != End(); i = Next(i))
			{
				if (GetValue(i) == value) keys.Add(GetKey(i));
			}
		}

		void Clear()
		{
			for (int i = 0; i < bucketCount; i++) buckets[i].State = HashBucketStateEmpty;
			count = 0;
		}

		TValue& operator [] (const TKey& key)
		{
			int x = TTraits::Hash(key) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == HashBucketStateUsed)
				{
					if (TTraits::Equals(buckets[x].Key, key)) return buckets[x].Value;
				}
				else if (buckets[x].State == HashBucketStateEmpty)
				{
					TValue v;
					Add(key, v);
					return Get(key);
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
				if (x == firstX)
				{
					rehash(bucketCount);
					return (*this)[key];
				}
			}
		}

		void Add(const TKey& key, const TValue& value)
		{
			if (count > (bucketCount/8)*5) expand();
			int x = TTraits::Hash(key) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == HashBucketStateUsed)
				{
					if (TTraits::Equals(buckets[x].Key, key)) XLI_BREAK_THROW("Map already contains the given key");
				}
				else if (buckets[x].State == HashBucketStateEmpty)
				{
					buckets[x].State = HashBucketStateUsed;
					buckets[x].Key = key;
					buckets[x].Value = value;
					count++;
					return;
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
				if (x == firstX)
				{
					rehash(bucketCount);
					Add(key, value);
					return;
				}
			}
		}

		void Add(const HashMap& h)
		{
			for (int i = h.Begin(); i != h.End(); i = h.Next(i))
			{
				Add(h.GetKey(i), h.GetValue(i));
			}
		}

		TValue Remove(const TKey& key)
		{
			int x = TTraits::Hash(key) & (bucketCount-1);

			while (true)
			{
				if (buckets[x].State == HashBucketStateUsed)
				{
					if (TTraits::Equals(buckets[x].Key, key))
					{
						buckets[x].State = HashBucketStateDummy;
						count--;
						return buckets[x].Value;
					}
				}
				else if (buckets[x].State == HashBucketStateEmpty)
				{
					XLI_BREAK_THROW("Map does not contain the given key");
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
			}
		}

		void Remove(const Array<TKey>& keys)
		{
			for (int i = 0; i < keys.Length(); i++) Remove(keys[i]);
		}

		bool ContainsKey(const TKey& key) const
		{
			int x = TTraits::Hash(key) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == HashBucketStateUsed)
				{
					if (TTraits::Equals(buckets[x].Key, key)) return true;
				}
				else if (buckets[x].State == HashBucketStateEmpty)
				{
					return false;
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
				if (x == firstX) { return false; }
			}
		}

		bool Get(const TKey& key, TValue& value) const
		{
			int x = TTraits::Hash(key) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == HashBucketStateUsed)
				{
					if (TTraits::Equals(buckets[x].Key, key))
					{
						value = buckets[x].Value;
						return true;
					}
				}
				else if (buckets[x].State == HashBucketStateEmpty)
				{
					return false;
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
				if (x == firstX) { return false; }
			}
		}
		/**
			Sets a key-value pair, and returns true if the key already existed.
			@return true if the key already existed, false otherwise.
		*/
		bool Set(const TKey& key, const TValue& value)
		{
			if (count > (bucketCount/8)*5) expand();
			int x = TTraits::Hash(key) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == HashBucketStateUsed)
				{
					if (TTraits::Equals(buckets[x].Key, key))
					{
						buckets[x].Value = value;
						return true;
					}
				}
				else if (buckets[x].State == HashBucketStateEmpty)
				{
					buckets[x].Key = key;
					buckets[x].State = HashBucketStateUsed;
					buckets[x].Value = value;
					count++;
					return false;
				}
				x += 1;
				if (x >= bucketCount) x -= bucketCount;
				if (x == firstX) rehash(bucketCount);
			}
		}

		TValue& Get(const TKey& key)
		{
			int x = TTraits::Hash(key) & (bucketCount-1);
			int firstX = x;
			while (true)
			{
				if (buckets[x].State == HashBucketStateUsed)
				{
					if (TTraits::Equals(buckets[x].Key, key))
					{
						return buckets[x].Value;
					}
				}
				else if (buckets[x].State == HashBucketStateEmpty)
				{
					XLI_BREAK_THROW("Map does not contain the given key");
				}
				x += 1;

				if (x >= bucketCount)
				{
					x -= bucketCount;
				}
				if (x == firstX)
				{
					XLI_BREAK_THROW("Map does not contain the given key");
				}
			}
		}

		int Count() const
		{
			return count;
		}
	};
}

#endif
