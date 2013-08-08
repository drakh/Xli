#ifndef __XLI_VECTOR2_H__
#define __XLI_VECTOR2_H__

#include <Xli/BaseTypes.h>
#include <Xli/Exception.h>
#include <Xli/Math.h>
#include <Xli/String.h>
#include <Xli/Hash.h>

namespace Xli
{
	/**
		\ingroup XliCoreMath
	*/
	template <typename T> class Vector2t
	{
	public:
		union
		{
			struct { T X, Y; };
			struct { T R, G; };
			T Comps[2];
		};

		T* Data()
		{
			return Comps;
		}

		const T* Data() const
		{
			return Comps;
		}

		operator T* ()
		{
			return Comps;
		}

		operator const T* () const
		{
			return Comps;
		}

		T& operator [] (int i)
		{
#ifdef XLI_RANGE_CHECK
			if (i >= 2 || i < 0)
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

			return Comps[i];
		}

		String ToString() const
		{
			return String(X) + ", " + Y;
		}

		Vector2t()
		{
		}

		Vector2t(const T& x, const T& y)
		{
			X = x;
			Y = y;
		}

		Vector2t(const Vector2t& v)
		{
			X = v.X;
			Y = v.Y;
		}

		template <typename U> explicit Vector2t(const Vector2t<U>& v)
		{
			X = (T)v.X;
			Y = (T)v.Y;
		}

		Vector2t& operator = (const Vector2t& v)
		{
			X = v.X;
			Y = v.Y;
			return *this;
		}

		bool operator == (const Vector2t& v) const
		{
			return X == v.X && Y == v.Y;
		}

		bool operator != (const Vector2t& v) const
		{
			return !(*this == v);
		}

		Vector2t operator - () const
		{
			Vector2t r;
			r.X = -X;
			r.Y = -Y;
			return r;
		}

		Vector2t operator + (const Vector2t& v) const
		{
			Vector2t r;
			r.X = X + v.X;
			r.Y = Y + v.Y;
			return r;
		}

		Vector2t operator - (const Vector2t& v) const
		{
			Vector2t r;
			r.X = X - v.X;
			r.Y = Y - v.Y;
			return r;
		}

		Vector2t operator * (const Vector2t& v) const
		{
			Vector2t r;
			r.X = X * v.X;
			r.Y = Y * v.Y;
			return r;
		}

		Vector2t operator / (const Vector2t& v) const
		{
			Vector2t r;
			r.X = X / v.X;
			r.Y = Y / v.Y;
			return r;
		}

		Vector2t operator + (const T& s) const
		{
			Vector2t r;
			r.X = X + s;
			r.Y = Y + s;
			return r;
		}

		Vector2t operator - (const T& s) const
		{
			Vector2t r;
			r.X = X - s;
			r.Y = Y - s;
			return r;
		}

		Vector2t operator * (const T& s) const
		{
			Vector2t r;
			r.X = X * s;
			r.Y = Y * s;
			return r;
		}

		Vector2t operator / (const T& s) const
		{
			Vector2t r;
			r.X = X / s;
			r.Y = Y / s;
			return r;
		}

		Vector2t& operator += (const Vector2t& v)
		{
			X += v.X;
			Y += v.Y;
			return *this;
		}

		Vector2t& operator -= (const Vector2t& v)
		{
			X -= v.X;
			Y -= v.Y;
			return *this;
		}

		Vector2t& operator *= (const Vector2t& v)
		{
			X *= v.X;
			Y *= v.Y;
			return *this;
		}

		Vector2t& operator /= (const Vector2t& v)
		{
			X /= v.X;
			Y /= v.Y;
			return *this;
		}

		Vector2t& operator += (const T& s)
		{
			X += s;
			Y += s;
			return *this;
		}

		Vector2t& operator -= (const T& s)
		{
			X -= s;
			Y -= s;
			return *this;
		}

		Vector2t& operator *= (const T& s)
		{
			X *= s;
			Y *= s;
			return *this;
		}

		Vector2t& operator /= (const T& s)
		{
			X /= s;
			Y /= s;
			return *this;
		}
	};

	/**
		\addtogroup XliCoreMath
		@{
	*/

	typedef Vector2t<float> Vector2;
	typedef Vector2t<double> Vector2d;
	typedef Vector2t<int> Vector2i;
	typedef Vector2t<unsigned int> Vector2u;

	typedef Vector2t<Int64> Vector2i64;
	typedef Vector2t<Int32> Vector2i32;
	typedef Vector2t<Int16> Vector2i16;
	typedef Vector2t<Int8> Vector2i8;
	typedef Vector2t<UInt64> Vector2u64;
	typedef Vector2t<UInt32> Vector2u32;
	typedef Vector2t<UInt16> Vector2u16;
	typedef Vector2t<UInt8> Vector2u8;

	static float Dot(const Vector2& v1, const Vector2& v2)
	{
		return v1.X * v2.X + v1.Y * v2.Y;
	}

	static float LengthSquared(const Vector2& v)
	{
		return Dot(v, v);
	}

	static float Length(const Vector2& v)
	{
		return Sqrt(Dot(v, v));
	}

	static Vector2 Normalize(const Vector2& v)
	{
		return v * (1.0f / Length(v));
	}

	static Vector2 Rotate(const Vector2& v, float angleRadians)
	{
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		Vector2 w;
		w.X = c * v.X + s * v.Y;
		w.Y = c * v.Y - s * v.X;

		return Normalize(w) * Length(v);
	}

	static Vector2 Vector2FromAngle(float angleRadians)
	{
		return Vector2(Cos(angleRadians), Sin(angleRadians));
	}

	static float Vector2ToAngle(const Vector2& v)
	{
		float linv = 1.0f / Length(v);
		return v.Y > 0.0f ? ArcCos(v.X * linv) : 2.0f * PIf - ArcCos(v.X * linv);
	}

	static float AngleBetween(const Vector2& v1, const Vector2& v2)
	{
		return ArcCos(Dot(Normalize(v1), Normalize(v2)));
	}

	/** @} */
}

#endif
