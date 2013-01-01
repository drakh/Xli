#ifndef __XLI_HASH_H__
#define __XLI_HASH_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
	/**
		\addtogroup Core
		@{
	*/

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

	/** @} */
}

#endif
