#ifndef __XLI_HASH_H__
#define __XLI_HASH_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
	/**
		\addtogroup Core
		@{
	*/

	UInt32 Hash(bool b);
	UInt32 Hash(void* p);
	UInt32 Hash(float f);
	UInt32 Hash(double d);

	UInt32 Hash(Int8 i);
	UInt32 Hash(UInt8 i);
	UInt32 Hash(Int16 i);
	UInt32 Hash(UInt16 i);
	UInt32 Hash(Int32 i);
	UInt32 Hash(UInt32 i);
	UInt32 Hash(Int64 i);
	UInt32 Hash(UInt64 i);

	template <class T> UInt32 Hash(const T& t) { return t.Hash(); }
	template <class T> UInt32 Hash(T& t) { return t.Hash(); }
	template <class T> UInt32 Hash(T* t) { return (UInt32)(long long)t; }

	/** @} */
}

#endif
