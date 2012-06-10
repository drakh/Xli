#ifndef __XLI_HASH_H__
#define __XLI_HASH_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
	inline UInt32 Hash(bool b) { return b ? 1 : 0; }
	inline UInt32 Hash(void* p) { return (UInt32)(long long)p; }
	inline UInt32 Hash(float f) { return *(UInt32*)&f; }
	inline UInt32 Hash(double d) { return ((UInt32*)&d)[0] ^ ((UInt32*)&d)[1]; }

	inline UInt32 Hash(Int8 i) { return (UInt32)i; }
	inline UInt32 Hash(UInt8 i) { return (UInt32)i; }
	inline UInt32 Hash(Int16 i) { return (UInt32)i; }
	inline UInt32 Hash(UInt16 i) { return (UInt32)i; }
	inline UInt32 Hash(Int32 i) { return (UInt32)i; }
	inline UInt32 Hash(UInt32 i) { return i; }
	inline UInt32 Hash(Int64 i) { return ((UInt32*)&i)[0] ^ ((UInt32*)&i)[1]; }
	inline UInt32 Hash(UInt64 i) { return ((UInt32*)&i)[0] ^ ((UInt32*)&i)[1]; }

	template <class T> UInt32 Hash(const T& t) { return t.Hash(); }
	template <class T> UInt32 Hash(T& t) { return t.Hash(); }
	template <class T> UInt32 Hash(T* t) { return (UInt32)(long long)t; }

	template <typename T1, typename T2> class HashTuple
	{
		T1 v1;
		T2 v2;

	public:
		HashTuple() 
		{
		}
		HashTuple(const T1& t1, const T2& t2): v1(t1), v2(t2) 
		{
		}
		HashTuple(const HashTuple& ht)
		{
			this->v1 = ht.v1;
			this->v2 = ht.v2;
		}
		bool operator == (const HashTuple& ht) const
		{
			return (v1 == ht.v1) && (v2 == ht.v2);
		}
		UInt32 Hash() const 
		{
			return Xli::Hash(v1) ^ Xli::Hash(v2);
		}
		T1 GetValue1()
		{
			return v1;
		}
		T2 GetValue2()
		{
			return v2;
		}
	};

	template <typename T1, typename T2> HashTuple<T1, T2> Tuple(const T1& v1, const T2& v2) { return HashTuple<T1, T2>(v1, v2); }
}

#endif
