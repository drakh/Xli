#ifndef __XLI_UTILS_H__
#define __XLI_UTILS_H__

namespace Xli
{
	template <typename T> inline void Swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	template <typename T> struct ComparatorLessThan
	{
		inline static bool Compare(const T& a, const T& b)
		{
			return a < b;
		}
	};
	template <typename T> struct ComparatorGreaterThan
	{
		inline static bool Compare(const T& a, const T& b)
		{
			return a > b;
		}
	};
	template <typename T> struct ComparatorPointerLessThan
	{
		inline static bool Compare(const T& a, const T& b)
		{
			return *a < *b;
		}
	};
	template <typename T> struct ComparatorPointerGreaterThan
	{
		inline static bool Compare(const T& a, const T& b)
		{
			return *a > *b;
		}
	};
}

#endif

