#ifndef __XLI_MATH_H__
#define __XLI_MATH_H__

#include "BaseTypes.h"
#include <cmath>
#include <cfloat>
#include <climits>

namespace Xli
{
	/**
		\addtogroup Math
		@{
	*/

	static const double PI = 3.1415926535897932384626433832795;
	static const double E = 2.71828182845904523536;

	static const float PIf = 3.1415926535897932384626433832795f;
	static const float Ef = 2.71828182845904523536f;

	static const float FloatZeroTolerance = 1e-4f;

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


	template <typename T> T SignPositiveZero(const T& x)
	{
		return (x < (T)0) ? (T)-1 : (T)1;
	}

	template <typename T> T Sign(const T& x)
	{
		return x == (T)-0 || x == (T)0 ? x : SignPositiveZero(x);
	}

	template <typename T> T Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	}

	template <typename T> T Max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	template <typename T> T Abs(const T& a)
	{
		return (a < (T)0) ? -a : a;
	}

	template <typename T> T Clamp(const T& x, const T& minimum, const T& maximum)
	{
		return Max(minimum, Min(maximum, x));
	}

	template <typename T> T Saturate(const T& x)
	{
		return Clamp(x, T(0), T(1));
	}


	static float Sin(float x)
	{
		return sinf(x);
	}

	static float Cos(float x)
	{
		return cosf(x);
	}

	static float Tan(float x)
	{
		return tanf(x);
	}

	static float ArcSin(float x)
	{
		return asinf(x);
	}

	static float ArcCos(float x)
	{
		return acosf(x);
	}

	static float ArcTan(float x)
	{
		return atanf(x);
	}

	static float Log(float x)
	{
		return logf(x);
	}

	static float Log10(float x)
	{
		return log10f(x);
	}

	static float Sqrt(float x)
	{
		return sqrtf(x);
	}

	static float Pow(float base, float exponent)
	{
		return powf(base, exponent);
	}

	static float Exp(float x)
	{
		return expf(x);
	}

	static float Fmod(float x, float y)
	{
		return fmodf(x, y);
	}


	static double Sin(double x)
	{
		return sin(x);
	}

	static double Cos(double x)
	{
		return cos(x);
	}

	static double Tan(double x)
	{
		return tan(x);
	}

	static double ArcSin(double x)
	{
		return asin(x);
	}

	static double ArcCos(double x)
	{
		return acos(x);
	}

	static double ArcTan(double x)
	{
		return atan(x);
	}

	static double Log(double x)
	{
		return log(x);
	}

	static double Log10(double x)
	{
		return log10(x);
	}

	static double Sqrt(double x)
	{
		return sqrt(x);
	}

	static double Pow(double base, double exponent)
	{
		return pow(base, exponent);
	}

	static double Exp(double x)
	{
		return exp(x);
	}

	static double Fmod(double x, double y)
	{
		return fmod(x, y);
	}


	template <typename T> T Log2(const T& x)
	{
		return Log(x) / Log((T)2.0);
	}

	template <typename T> T Log(const T& x, const T& base)
	{
		return Log(x) / Log(base);
	}

	template <typename T> T Floor(const T& x)
	{
		return (T)((x >= (T)0.0)? (int)x: (int)x - 1);
	}

	template <typename T> T Ceil(const T& x)
	{
		return (T)((x > (T)0.0)? (int)x + 1: (int)x);
	}

	template <typename T> T Round(const T& x)
	{
		return Floor(x + (T)0.5);
	}

	template <typename T> T Frac(const T& x)
	{
		return x - Floor(x);
	}


	static float RadToDeg(float x)
	{
		return x * 180.0f / (float)PI;
	}

	static float DegToRad(float x)
	{
		return x * (float)PI / 180.0f;
	}


	template <typename T> float Map(const T& a, const T& b, const T& t)
	{
		return float(t - a) / float(b - a);
	}

	template <typename T> T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	template <typename T> T SCurve(const T& a, const T& b, float t)
	{
		return a + (b - a) * (0.5f - Cos(t * (float)PI) * 0.5f);
	}

	template <typename T> T Ramp(const T& a, const T& b, float t)
	{
		return a + (b - a) * (t*t);
	}

	template <typename T> T InterpolateCatmullRom(const T& a, const T& b, const T& c, const T& d, float t)
	{
		T p = b*3.0f - c*3.0f + d - a;
		T q = a*2.0f - b*5.0f + c*4.0f - d;
		T r = c - a;
		T s = b;
		return (p*(t*t*t) + q*(t*t) + r*t)*0.5f + s;
	}

	template <typename T> T InterpolateCubic(const T& a, const T& b, const T& c, const T& d, float t)
	{
		T p = (d - c) - (a - b);
		T q = (a - b) - p;
		T r = c - a;
		T s = b;
		return p*(t*t*t) + q*(t*t) + r*t + s;
	}


	static int UpperPow2(int x)
	{
		int y = x - 1;
		y = y | (y >> 1);
		y = y | (y >> 2);
		y = y | (y >> 4);
		y = y | (y >> 8);
		y = y | (y >> 16);
		return y + 1;
	}

	static bool IsPow2(int x)
	{
		return (x == (x & -x));
	}

	/** @} */
}

#endif
