#ifndef __XLI_SATURATED_H__
#define __XLI_SATURATED_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
	/**
		\ingroup Media

		Wraps another type into a saturated type.
		This template will saturate the wrapped value into the given range.
		T is the type to wrap.
		TLong is a type of the same principal type as T, but with enough range to hold any arithmetic result
		of operations on T.
		MinValue is the minimum value of the saturated type.
		MaxValue is the maximum value of the saturated type. 
		The min/max values can differ from the actual min/max values of T.
		Using this type as float will return it in a normalized 0..1 range.
		When constructing this type from float it will be interpreted from a 0..1 range.
	*/
	template <typename T, typename TLong, TLong MaxValue> class Saturated
	{
	public:

		T Value;
		Saturated() {}

		static T Saturate(TLong v)
		{
			if (v > MaxValue) return (T)MaxValue;
			if (v < 0) return 0;
			return (T)v;
		}

		Saturated& operator = (float f) { Value = Saturate((TLong)f*MaxValue); return *this; }
		Saturated& operator = (const T& v) { Value = v; return *this; }

		template <typename TOp> T operator + (const TOp& v)	{ return Saturate((TLong)Value + (TLong)v.Value); }
		template <typename TOp> T operator - (const TOp& v)	{ return Saturate((TLong)Value - (TLong)v.Value); }
		template <typename TOp> T operator * (const TOp& v)	{ return Saturate(((TLong)Value * (TLong)v.Value) / MaxValue); }
		template <typename TOp> T operator / (const TOp& v)	{ return Saturate(((TLong)Value * MaxValue) / (TLong)v.Value); }

		template <typename TOp> T& operator += (const TOp& v) { return (Value = Value + v.Value); }
		template <typename TOp> T& operator -= (const TOp& v) { return (Value = Value + v.Value); }
		template <typename TOp> T& operator *= (const TOp& v) { return (Value = Value * v.Value); }
		template <typename TOp> T& operator /= (const TOp& v) { return (Value = Value + v.Value); }

		operator float() const { return (float)Value/(float)MaxValue; }

		operator T() { return Value; }
		T& operator ->() { return Value; }

		bool operator > (const Saturated& s) const { return Value > s.Value; }
		bool operator < (const Saturated& s) const { return Value < s.Value; }
		bool operator >= (const Saturated& s) const { return Value >= s.Value; }
		bool operator <= (const Saturated& s) const { return Value <= s.Value; }
		bool operator == (const Saturated& s) const { return Value == s.Value; }
		bool operator != (const Saturated& s) const { return Value != s.Value; }
	};

	typedef Saturated<UInt8, long, 0xFF> SaturatedUInt8;
	typedef Saturated<UInt16, long, 0xFFFF> SaturatedUInt16;
	typedef Saturated<UInt32, long, 0xFFFFFFFF> SaturatedUInt32;
}

#endif
