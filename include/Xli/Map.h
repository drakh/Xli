#ifndef __XLI_MAP_H__
#define __XLI_MAP_H__

#include <Xli/Array.h>

namespace Xli
{
    /**
        \ingroup XliCoreContainers

        Map which preserves the order of the keys. This implementation has slower look up than HashMap and is not meant to be used in performance critical code.
    */
    template <typename TKey, typename TValue, int TBufSize = 4> class Map
    {
        Array<TKey, TBufSize> keys;
        Array<TValue, TBufSize> values;

    public:
        /**
            Gives an integer iterator to the beginning of the map.
        */
        int Begin() const
        {
            return 0;
        }

        /**
            Returns the iterator value that indicates the end of the map.
        */
        int End() const
        {
            return keys.Length();
        }

        /**
            Advances an iterator to the next element.
            @see Begin
            @param iterator The current iterator
            @return An iterator pointing to the succeeding element, or End().
        */
        int Next(int iterator) const
        {
            return iterator + 1;
        }

        /**
            Returns the value at the given iterator
            Use Begin(), Next() and End() to iterate through the map.
            @param iterator The iterator at which to look up the value
            @return The value at the given iterator.
        */
        const TValue& GetValue(int iterator) const
        {
            return values[iterator];
        }

        /**
            Returns the value at the given iterator
            Use Begin(), Next() and End() to iterate through the map.
            @param iterator The iterator at which to look up the value
            @return The value at the given iterator.
        */
        TValue& GetValue(int iterator)
        {
            return values[iterator];
        }

        /**
            Sets a new value at the given iterator
            Use Begin(), Next() and End() to iterate through the map.
            @param iterator The iterator at which to look up the value
            @param value The new value to be set at the given iterator.
        */
        void SetValue(int iterator, const TValue& value)
        {
            values[iterator] = value;
        }

        /**
            Returns the key at the given iterator
            Use Begin(), Next() and End() to iterate through the map.
            @param iterator The iterator at which to look up the key
            @return The key at the given iterator.
        */
        const TKey& GetKey(int iterator) const
        {
            return keys[iterator];
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
        bool TryGetKeyFromValue(const TValue& value, TKey& result) const
        {
            for (int i = Begin(); i != End(); i = Next(i))
            {
                if (GetValue(i) == value)
                {
                    result = GetKey(i);
                    return true;
                }
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
                if (GetValue(i) == value) 
                    keys.Add(GetKey(i));
        }

        void Clear()
        {
            keys.Clear();
            values.Clear();
        }

        TValue& operator [] (const TKey& key)
        {
            int i = keys.IndexOf(key);
            
            if (i == -1) 
                XLI_THROW("Map does not contain the given key");
            
            return values[i];
        }

        void Add(const TKey& key, const TValue& value)
        {
            if (keys.Contains(key))
                XLI_THROW("Map already contains the given key");

            keys.Add(key);
            values.Add(value);
        }

        bool Remove(const TKey& key)
        {
            int i = keys.IndexOf(key);
            
            if (i == -1) 
                return false;
            
            TValue value = values[i];
            keys.RemoveAt(i);
            values.RemoveAt(i);
            return true;
        }

        bool ContainsKey(const TKey& key) const
        {
            return keys.Contains(key);
        }

        bool TryGetValue(const TKey& key, TValue& value) const
        {
            int i = keys.IndexOf(key);
            
            if (i == -1) 
                return false;
            
            value = values[i];
            return true;
        }

        int Count() const
        {
            return keys.Length();
        }
    };
}

#endif
