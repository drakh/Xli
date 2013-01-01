#ifndef __XLI_UTILS_H__
#define __XLI_UTILS_H__

namespace Xli
{
	/**
		\addtogroup Utils
		@{
	*/
	template <typename T> inline void Swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	/** @} */

	/**
		\ingroup Utils
	*/
	template <typename T> struct ComparatorLessThan
	{
		inline static bool Compare(const T& a, const T& b)
		{
			return a < b;
		}
	};

	/**
		\ingroup Utils
	*/
	template <typename T> struct ComparatorGreaterThan
	{
		inline static bool Compare(const T& a, const T& b)
		{
			return a > b;
		}
	};

	/**
		\ingroup Utils
	*/
	template <typename T> struct ComparatorPointerLessThan
	{
		inline static bool Compare(const T& a, const T& b)
		{
			return *a < *b;
		}
	};

	/**
		\ingroup Utils
	*/
	template <typename T> struct ComparatorPointerGreaterThan
	{
		inline static bool Compare(const T& a, const T& b)
		{
			return *a > *b;
		}
	};

	/**
		\ingroup Utils
	*/
	class Endian
	{
	public:
		template <typename T> static T Swap4(const T& i)
		{
			T r;
			UInt8 *src = (UInt8*)&i, *dst = (UInt8*)&r;
			dst[0] = src[3];
			dst[1] = src[2];
			dst[2] = src[1];
			dst[3] = src[0];
			return r;
		}
		template <typename T> static T Swap2(const T& i)
		{
			T r;
			UInt8 *src = (UInt8*)&i, *dst = (UInt8*)&r;
			dst[0] = src[1];
			dst[1] = src[0];
			return r;
		}
	};
}

#endif

