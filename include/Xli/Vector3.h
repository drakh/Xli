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
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
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
		/** Used to construct a vector when const T& is not possible for arguments (managed bindings etc.) */
		static Vector3t From(T x, T y, T z)
		{
			Vector3t v;
			v.X = x;
			v.Y = y;
			v.Z = z;
			return v;
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
			return *this * ((T)1.0 / s);
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
			return *this *= (T(1.0) / s);
		}


		Vector3t Cross(const Vector3t& v) const
		{
			Vector3t r;
			r.X = Y*v.Z - Z*v.Y;
			r.Y = Z*v.X - X*v.Z;
			r.Z = X*v.Y - Y*v.X;
			return r;
		}
		T Dot(const Vector3t& v) const
		{
			return X*v.X + Y*v.Y + Z*v.Z;
		}
		T LengthSquared() const
		{
			return Dot(*this);
		}
		T Length() const
		{
			return Sqrt(LengthSquared());
		}
		void Normalize()
		{
			*this /= Length();
		}
		Vector3t Normalized() const
		{
			return *this / Length();
		}

		bool operator == (const Vector3t& v) const
		{
			return (X == v.X) && (Y == v.Y) && (Z == v.Z);
		}

		bool operator != (const Vector3t& v) const
		{
			return !(*this == v);
		}

		Vector3t Rotated(Vector3t axis, const T& angleRadians) const
		{    
			Vector3t w;
			axis.Normalize();

			// calculate parameters of the rotation matrix
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);
			T t = 1 - c;

			// multiply v with rotation matrix
			w.X = (t * axis.X * axis.X +          c) * X
				+ (t * axis.X * axis.Y + s * axis.Z) * Y
				+ (t * axis.X * axis.Z - s * axis.Y) * Z;

			w.Y = (t * axis.X * axis.Y - s * axis.Z) * X 
				+ (t * axis.Y * axis.Y +          c) * Y 
				+ (t * axis.Y * axis.Z + s * axis.X) * Z;

			w.Z = (t * axis.X * axis.Z + s * axis.Y) * X 
				+ (t * axis.Y * axis.Z - s * axis.X) * Y 
				+ (t * axis.Z * axis.Z +          c) * Z;

			w.Normalize();
			w = w * Length();

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

	/** @} */
}

#endif
