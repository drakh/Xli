
#ifndef __XLI_DICTIONARY_H__
#define __XLI_DICTIONARY_H__

#include <Xli/HashMap.h>
#include <Xli/Managed.h>

namespace Xli
{
	/**
		Dictionary template.
		Wraps a HashMap into a managed pass-by-reference type.
	*/
	template <typename TKey, typename TValue, typename TTraits = HashMapDefaultTraits<TKey, TValue> > class Dictionary
	{
		HashMap<TKey, TValue, TTraits>* map;

	public:
		Dictionary()
		{
			map = 0;
		}

		Dictionary(Dictionary& dict)
		{
			map = dict.map;
			if (map) map->AddRef();
		}

		inline Dictionary& operator = (const Dictionary& dict)
		{
			if (map) map->Release();
			this->map = dict.map;
			if (map) map->AddRef();
			return *this;
		}

		~Dictionary()
		{
			if (map) map->Release();
		}

		int Begin()
		{
			if (!map) return -1;
			return map->Begin();
		}

		int End()
		{
			return -1;
		}

		int Next(int iterator)
		{
			if (!map) XLI_THROW("Dictionary::Next(): invalid iterator");
			return map->Next();
		}

		TValue GetValue(int iterator)
		{
			if (!map) XLI_THROW("Dictionary::GetValue(): invalid iterator");
			return map->GetValue(iterator);
		}

		TValue GetKey(int iterator)
		{
			if (!map) XLI_THROW("Dictionary::GetKey(): invalid iterator");
			return map->GetKey(iterator);
		}

		void Clear()
		{
			if (map) map->Release();
			map = 0;
		}

		TValue& operator [] (TKey key)
		{
			if (!map) map = new HashMap<TKey, TValue, TTraits>();
			HashMap<TKey, TValue, TTraits>& m = *map;
			return m[key];
		}

		void Add(TKey key, TValue value)
		{
			if (!map) map = new HashMap<TKey, TValue, TTraits>();
			map->Add(key, value);
		}

		void Remove(TKey key)
		{
			if (!map) XLI_THROW("Dictionary::Remove(): dictionary is empty");
			map->Remove(key);
		}

		bool ContainsKey(TKey key)
		{
			if (!map) return false;
			return map->ContainsKey(key);
		}

		bool Get(TKey key, TValue& value)
		{
			if (!map) return false;
			return map->Get(key, value);
		}

		/**
			Sets a key-value pair, and returns true if the key allready existed.
			@return true if the key allready existed, false otherwise.
		*/
		bool Set(TKey key, TValue value)
		{
			if (!map) map = new HashMap<TKey, TValue, TTraits>();
			return map->Set(key, value);
		}

		int Count()
		{
			if (!map) return 0;
			return map->Count();
		}
	};



}

#endif
