#ifndef __XLI_VECTOR2_H__
#define __XLI_VECTOR2_H__

#include "Xli/BaseTypes.h"
#include "Math.h"
#include "String.h"
#include "Hash.h"

namespace Xli
{
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
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			return Comps[i];
		}
		String ToString() const
		{
			return String(X) + ", " + Y;
		}
		UInt32 Hash() const
		{
			return Xli::Hash(X) ^ Xli::Hash(Y);
		}
		bool operator == (const Vector2t& v) const
		{
			return X == v.X && Y == v.Y;
		}
		bool operator != (const Vector2t& v) const
		{
			return !(*this == v);
		}

		Vector2t()
		{
		}
		Vector2t(const T& x, const T& y)
		{
			X = x;
			Y = y;
		}
		/** Used to construct a vector when const T& is not possible for arguments (managed bindings etc.) */
		static Vector2t From(T x, T y)
		{
			Vector2t v;
			v.X = x;
			v.Y = y;
			return v;
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
		template <typename U> operator Vector2t<U>() const
		{
			Vector2t<U> r;
			r.X = (U)X;
			r.Y = (U)Y;
			return r;
		}
		Vector2t& operator = (const Vector2t& v)
		{
			X = v.X;
			Y = v.Y;
			return *this;
		}

	/*	Vector2t operator - () const
		{
			Vector2t r;
			r.X = -v.X;
			r.Y = -v.Y;
			return r;
		}*/

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


		T Dot(const Vector2t& v) const
		{
			return X*v.X + Y*v.Y;
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
		Vector2t Normalized() const
		{
			return *this / Length();
		}

		Vector2t Rotated(T angleRadians) const
		{
			Vector2t w;

			float c = Cos(angleRadians);
			float s = Sin(angleRadians);

			w.X = c*X + s*Y;
			w.Y = c*Y - s*X;
			w.Normalize();
			w = w * Length();

			return w;
		}

		static Vector2t FromAngle(const T& angleRadians)
		{
			return Vector2t(Cos(angleRadians), Sin(angleRadians));
		}

		T ToAngle() const
		{
			if (Y > 0) return ArcCos(X);
			else return (T)(2.0*PI) - ArcCos(X);
		}

		T AngleBetween(const Vector2t& v) const
		{
			return ArcCos((*this).Normalized().Dot(v.Normalized()));
		}
	};

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
}

#endif
