#ifndef __XLI_VECTOR3_H__
#define __XLI_VECTOR3_H__

#include "Vector2.h"

namespace Xli
{
	/**
		\ingroup Math
	*/
	template <typename T> class Vector3t
	{
	public:
		union
		{
			struct { T X, Y, Z; };
			struct { T R, G, B; };
			T Comps[3];
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
			if (i >= 3 || i < 0)
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif

			return Comps[i];
		}

		String ToString() const
		{
			return String(X) + ", " + Y + ", " + Z;
		}

		Vector2t<T> XY() const
		{
			return Vector2t<T>(X, Y);
		}

		Vector3t(const Vector2t<T>& xy, T z)
		{
			X = xy.X;
			Y = xy.Y;
			Z = z;
		}

		Vector3t()
		{
		}

		Vector3t(const T& x, const T& y, const T& z)
		{
			X = x;
			Y = y;
			Z = z;
		}

		Vector3t(const Vector3t& v)
		{
			X = v.X;
			Y = v.Y;
			Z = v.Z;
		}

		template <typename U> explicit Vector3t(const Vector3t<U>& v)
		{
			X = (T)v.X;
			Y = (T)v.Y;
			Z = (T)v.Z;
		}

		template <typename U> operator Vector3t<U>() const
		{
			Vector3t<U> r;
			r.X = X;
			r.Y = Y;
			r.Z = Z;
			return r;
		}

		bool operator == (const Vector3t& v) const
		{
			return (X == v.X) && (Y == v.Y) && (Z == v.Z);
		}

		bool operator != (const Vector3t& v) const
		{
			return !(*this == v);
		}

		Vector3t& operator = (const Vector3t& v)
		{
			X = v.X;
			Y = v.Y;
			Z = v.Z;
			return *this;
		}

		Vector3t operator - () const
		{
			Vector3t r;
			r.X = -X;
			r.Y = -Y;
			r.Z = -Z;
			return r;
		}

		Vector3t operator + (const Vector3t& v) const
		{
			Vector3t r;
			r.X = X + v.X;
			r.Y = Y + v.Y;
			r.Z = Z + v.Z;
			return r;
		}

		Vector3t operator - (const Vector3t& v) const
		{
			Vector3t r;
			r.X = X - v.X;
			r.Y = Y - v.Y;
			r.Z = Z - v.Z;
			return r;
		}

		Vector3t operator * (const Vector3t& v) const
		{
			Vector3t r;
			r.X = X * v.X;
			r.Y = Y * v.Y;
			r.Z = Z * v.Z;
			return r;
		}

		Vector3t operator / (const Vector3t& v) const
		{
			Vector3t r;
			r.X = X / v.X;
			r.Y = Y / v.Y;
			r.Z = Z / v.Z;
			return r;
		}

		Vector3t operator + (const T& s) const
		{
			Vector3t r;
			r.X = X + s;
			r.Y = Y + s;
			r.Z = Z + s;
			return r;
		}

		Vector3t operator - (const T& s) const
		{
			Vector3t r;
			r.X = X - s;
			r.Y = Y - s;
			r.Z = Z - s;
			return r;
		}

		Vector3t operator * (const T& s) const
		{
			Vector3t r;
			r.X = X * s;
			r.Y = Y * s;
			r.Z = Z * s;
			return r;
		}

		Vector3t operator / (const T& s) const
		{
			Vector3t r;
			r.X = X / s;
			r.Y = Y / s;
			r.Z = Z / s;
			return r;
		}

		Vector3t& operator += (const Vector3t& v)
		{
			X += v.X;
			Y += v.Y;
			Z += v.Z;
			return *this;
		}

		Vector3t& operator -= (const Vector3t& v)
		{
			X -= v.X;
			Y -= v.Y;
			Z -= v.Z;
			return *this;
		}

		Vector3t& operator *= (const Vector3t& v)
		{
			X *= v.X;
			Y *= v.Y;
			Z *= v.Z;
			return *this;
		}

		Vector3t& operator /= (const Vector3t& v)
		{
			X /= v.X;
			Y /= v.Y;
			Z /= v.Z;
			return *this;
		}

		Vector3t& operator += (const T& s)
		{
			X += s;
			Y += s;
			Z += s;
			return *this;
		}

		Vector3t& operator -= (const T& s)
		{
			X -= s;
			Y -= s;
			Z -= s;
			return *this;
		}

		Vector3t& operator *= (const T& s)
		{
			X *= s;
			Y *= s;
			Z *= s;
			return *this;
		}

		Vector3t& operator /= (const T& s)
		{
			X /= s;
			Y /= s;
			Z /= s;
			return *this;
		}

		/*
		static Vector3t Rotate(const Vector3t& v, Vector3t axis, T angleRadians)
		{  
			Vector3t w;
			axis = Normalize(axis);

			// calculate parameters of the rotation matrix
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);
			T t = 1 - c;

			// multiply v with rotation matrix
			w.X = (t * axis.X * axis.X +          c) * v.X
				+ (t * axis.X * axis.Y + s * axis.Z) * v.Y
				+ (t * axis.X * axis.Z - s * axis.Y) * v.Z;

			w.Y = (t * axis.X * axis.Y - s * axis.Z) * v.X 
				+ (t * axis.Y * axis.Y +          c) * v.Y 
				+ (t * axis.Y * axis.Z + s * axis.X) * v.Z;

			w.Z = (t * axis.X * axis.Z + s * axis.Y) * v.X 
				+ (t * axis.Y * axis.Z - s * axis.X) * v.Y 
				+ (t * axis.Z * axis.Z +          c) * v.Z;

			w = Normalize(w);
			w = w * v.Length();

			return w;
		}

		void Rotate(const Vector3t& axis, const T& angleRadians)
		{
			*this = Rotated(axis, angleRadians);
		}

		T AngleBetween(const Vector3t& v) const
		{
			return ArcCos((*this).Normalized().Dot(v.Normalized()));
		}
		*/
	};

	/**
		\addtogroup Math
		@{
	*/

	typedef Vector3t<float> Vector3;
	typedef Vector3t<double> Vector3d;
	typedef Vector3t<int> Vector3i;
	typedef Vector3t<unsigned int> Vector3u;

	typedef Vector3t<Int64> Vector3i64;
	typedef Vector3t<Int32> Vector3i32;
	typedef Vector3t<Int16> Vector3i16;
	typedef Vector3t<Int8> Vector3i8;
	typedef Vector3t<UInt64> Vector3u64;
	typedef Vector3t<UInt32> Vector3u32;
	typedef Vector3t<UInt16> Vector3u16;
	typedef Vector3t<UInt8> Vector3u8;

	static inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.Y * v2.Z - v1.Z * v2.Y,
		               v1.Z * v2.X - v1.X * v2.Z,
		               v1.X * v2.Y - v1.Y * v2.X);
	}

	static inline float Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
	}

	static inline float LengthSquared(const Vector3& v)
	{
		return Dot(v, v);
	}

	static inline float Length(const Vector3& v)
	{
		return Sqrt(LengthSquared(v));
	}

	static inline Vector3 Normalize(const Vector3& v)
	{
		return v / Length(v);
	}

	/** @} */
}

#endif
