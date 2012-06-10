#ifndef __XLI_STRING_H__
#define __XLI_STRING_H__

#include <Xli/Exception.h>
#include <Xli/Array.h>

namespace Xli
{
	template <typename T, uint TBufSize> int Stringt<T, TBufSize>::LastIndexOf(T c, int start) const
	{
#ifdef XLI_RANGE_CHECK
		if (start >= length)
		{
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
		}
#endif
		for (int i = start; i >= 0; i--)
		{
			if (data[i] == c) return i;
		}
		return -1;
	}

	template <typename T, uint TBufSize> T& Stringt<T, TBufSize>::Get(int index)
	{
#ifdef XLI_RANGE_CHECK
		if ((uint)index >= length)
		{
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
		}
#endif
		return data[index];
	}

	template <typename T, uint TBufSize> const T& Stringt<T, TBufSize>::Get(int index) const
	{
#ifdef XLI_RANGE_CHECK
		if ((uint)index >= length)
		{
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
		}
#endif
		return data[index];
	}

	template <typename T, uint TBufSize> Stringt<T, TBufSize> Stringt<T, TBufSize>::SubString(int start, int len) const
	{
#ifdef XLI_RANGE_CHECK
		if (start + len > (int)length || start < 0)
		{
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
		}
#endif
		return Stringt(data + start, len);
	}

	template <typename T, uint TBufSize> void Stringt<T, TBufSize>::Split(T c, Array<Stringt<T, TBufSize> >& parts) const
	{
		uint start = 0;

		for (uint i = 0; i < length; i++)
		{
			if (data[i] == c)
			{
				parts.Add(SubString(start, i-start));
				start = i+1;
			}
		}

		if (start < length)
		{
			parts.Add(SubString(start, length-start));
		}
	}

	template <typename T, uint TBufSize> Stringt<T, TBufSize> Stringt<T, TBufSize>::Join(T c, const Array<Stringt<T, TBufSize> >& list)
	{
		Stringt r;
		r.length = -1;
		for (int i = 0; i < list.Length(); i++) r.length += list[i].Length() + 1;
		if (r.length == -1) return Stringt();

		r.data = new T[r.length+1];
		int p = 0;

		for (int i = 0; i < list.Length(); i++)
		{
			StringTools::Copy(r.data+p, list[i].Data(), list[i].Length());
			p += list[i].Length();
			r.data[p++] = c;
		}

		r.data[r.length] = 0;
		return r;
	}
}

#endif
