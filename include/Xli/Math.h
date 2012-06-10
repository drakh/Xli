#ifndef __XLI_MATH_H__
#define __XLI_MATH_H__

#include "BaseTypes.h"
#include <cmath>
#include <cfloat>
#include <climits>

namespace Xli
{
	static const double PI = 3.1415926535897932384626433832795;
	static const double TwoPI = 2.0 * PI;
	static const double HalfPI = 0.5 * PI;
	static const double E = 2.71828182845904523536;

	static const int IntMax = INT_MAX;
	static const int IntMin = INT_MIN;
	static const float FloatMax = FLT_MAX;
	static const float FloatMin = FLT_MIN;
	static const double DoubleMax = DBL_MAX;
	static const double DoubleMin = DBL_MIN;

	static const Int8 Int8Max = 0x7f;
	static const UInt8 UInt8Max = 0xff;

	static const Int16 Int16Max = 0x7fff;
	static const UInt16 UInt16Max = 0xffff;

	static const Int32 Int32Max = 0x7fffffff;
	static const UInt32 UInt32Max = 0xffffffff;

	static const Int64 Int64Max = 0x7fffffffffffffffll;
	static const UInt64 UInt64Max = 0xffffffffffffffffull;


	template <typename T> inline T SignPositiveZero(const T& x)
	{
		return (x < (T)0)? (T)-1: (T)1;
	}
	template <typename T> inline T Sign(const T& x)
	{
		return x==(T)-0||x==(T)0? x : SignPositiveZero(x);
	}
	template <typename T> inline T Min(const T& a, const T& b)
	{
		return (a < b)? a: b;
	}
	template <typename T> inline T Max(const T& a, const T& b)
	{
		return (a > b)? a: b;
	}
	template <typename T> inline T Abs(const T& a)
	{
		return (a < (T)0)? -a: a;
	}
	template <typename T> inline T Clamp(const T& x, const T& minimum, const T& maximum)
	{
		return Max(minimum, Min(maximum, x));
	}
	template <typename T> inline T Saturate(const T& x)
	{
		return Clamp(x, T(0), T(1));
	}


	inline float Sin(const float& x)
	{
		return sinf(x);
	}
	inline float Cos(const float& x)
	{
		return cosf(x);
	}
	inline float Tan(const float& x)
	{
		return tanf(x);
	}
	inline float ArcSin(const float& x)
	{
		return asinf(x);
	}
	inline float ArcCos(const float& x)
	{
		return acosf(x);
	}
	inline float ArcTan(const float& x)
	{
		return atanf(x);
	}
	inline float Log(const float& x)
	{
		return logf(x);
	}
	inline float Log10(const float& x)
	{
		return log10f(x);
	}
	inline float Sqrt(const float& x)
	{
		return sqrtf(x);
	}
	inline float Pow(const float& base, const float& exponent)
	{
		return powf(base, exponent);
	}
	inline float Exp(const float& x)
	{
		return expf(x);
	}
	inline float Fmod(const float& x, const float& y)
	{
		return fmodf(x, y);
	}


	inline double Sin(const double& x)
	{
		return sin(x);
	}
	inline double Cos(const double& x)
	{
		return cos(x);
	}
	inline double Tan(const double& x)
	{
		return tan(x);
	}
	inline double ArcSin(const double& x)
	{
		return asin(x);
	}
	inline double ArcCos(const double& x)
	{
		return acos(x);
	}
	inline double ArcTan(const double& x)
	{
		return atan(x);
	}
	inline double Log(const double& x)
	{
		return log(x);
	}
	inline double Log10(const double& x)
	{
		return log10(x);
	}
	inline double Sqrt(const double& x)
	{
		return sqrt(x);
	}
	inline double Pow(const double& base, const double& exponent)
	{
		return pow(base, exponent);
	}
	inline double Exp(const double& x)
	{
		return exp(x);
	}
	inline double Fmod(const double& x, const double& y)
	{
		return fmod(x, y);
	}


	template <typename T> inline T Log2(const T& x)
	{
		return Log(x) / Log((T)2.0);
	}
	template <typename T> inline T Log(const T& x, const T& base)
	{
		return Log(x) / Log(base);
	}
	template <typename T> inline T Floor(const T& x)
	{
		return (T)((x >= (T)0.0)? (int)x: (int)x - 1);
	}
	template <typename T> inline T Ceil(const T& x)
	{
		return (T)((x > (T)0.0)? (int)x + 1: (int)x);
	}
	template <typename T> inline T Round(const T& x)
	{
		return Floor(x + (T)0.5);
	}
	template <typename T> inline T Frac(const T& x)
	{
		return x - Floor(x);
	}


	inline float RadToDeg(const float& x)
	{
		return x * 180.0f / (float)PI;
	}
	inline float DegToRad(const float& x)
	{
		return x * (float)PI / 180.0f;
	}


	inline int UpperPow2(int x)
	{
		int y = x - 1;
		y = y | (y >> 1);
		y = y | (y >> 2);
		y = y | (y >> 4);
		y = y | (y >> 8);
		y = y | (y >> 16);
		return y + 1;
	}

	inline bool IsPow2(int x)
	{
		return (x == (x & -x));
	}
}

#endif
