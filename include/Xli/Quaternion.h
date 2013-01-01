#ifndef __XLI_QUATERNION_H__
#define __XLI_QUATERNION_H__

#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace Xli
{
	/**
		\ingroup Math
	*/
	template <typename T> class Quaterniont
	{
	public:
		union
		{
			struct { T X, Y, Z, W; };
			T Coord[4];
		};

		operator T*()
		{
			return Coord;
		}
		T& operator [] (int i)
		{
#ifdef XLI_RANGE_CHECK
			if (i >= 4 || i < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			return Coord[i];
		}
		String ToString() const
		{
			return String(X) + ", " + Y + ", " + Z + ", " + W;
		}


		Quaterniont()
		{
		}
		Quaterniont(const T& x, const T& y, const T& z, const T& w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}
		Quaterniont(const Vector4t<T>& v)
		{
			X = v.X;
			Y = v.Y;
			Z = v.Z;
			W = v.W;
		}
		Quaterniont(const Quaterniont& q)
		{
			X = q.X;
			Y = q.Y;
			Z = q.Z;
			W = q.W;
		}
		template <typename U> explicit Quaterniont(const Quaterniont<U>& q)
		{
			X = (T)q.X;
			Y = (T)q.Y;
			Z = (T)q.Z;
			W = (T)q.W;
		}
		template <typename U> operator Quaterniont<U>() const
		{
			return Quaterniont<U>(X, Y, Z, W);
		}
		Quaterniont& operator = (const Quaterniont& v)
		{
			X = v.X;
			Y = v.Y;
			Z = v.Z;
			W = v.W;
			return *this;
		}

		Quaterniont(const Vector3t<T>& axis, const T& angleRadians)
		{
			T d = axis.Length();
			if (d == T(0.0)) return;
			T s = Sin(angleRadians * T(0.5)) / d;

			X = axis.X * s;
			Y = axis.Y * s;
			Z = axis.Z * s;
			W = Cos(angleRadians * T(0.5));
		}

		Quaterniont(const T& yaw, const T& pitch, const T& roll)
		{
			T halfYaw = yaw * T(0.5);
			T halfPitch = pitch * T(0.5);
			T halfRoll = roll * T(0.5);

			T cosYaw = Cos(halfYaw);
			T sinYaw = Sin(halfYaw);
			T cosPitch = Cos(halfPitch);
			T sinPitch = Sin(halfPitch);
			T cosRoll = Cos(halfRoll);
			T sinRoll = Sin(halfRoll);

			X = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
			Y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
			Z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
			W = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		}

		/*Quaterniont operator - () const
		{
			Quaterniont r;
			r.X = -q.X;
			r.Y = -q.Y;
			r.Z = -q.Z;
			r.W = -q.W;
			return r;
		}*/
		Quaterniont operator + (const Quaterniont& q) const
		{
			Quaterniont r;
			r.X = X + q.X;
			r.Y = Y + q.Y;
			r.Z = Z + q.Z;
			r.W = W + q.W;
			return r;
		}
		Quaterniont operator - (const Quaterniont& q) const
		{
			Quaterniont r;
			r.X = X - q.X;
			r.Y = Y - q.Y;
			r.Z = Z - q.Z;
			r.W = W - q.W;
			return r;
		}
		Quaterniont operator * (const T& s) const
		{
			Quaterniont r;
			r.X = X * s;
			r.Y = Y * s;
			r.Z = Z * s;
			r.W = W * s;
			return r;
		}
		Quaterniont operator / (const T& s) const
		{
			return *this * (T(1.0) / s);
		}

		Quaterniont& operator += (const Quaterniont& q)
		{
			X += q.X;
			Y += q.Y;
			Z += q.Z;
			W += q.W;
			return *this;
		}
		Quaterniont& operator -= (const Quaterniont& q)
		{
			X -= q.X;
			Y -= q.Y;
			Z -= q.Z;
			W -= q.W;
			return *this;
		}
		Quaterniont& operator *= (const Quaterniont& q)
		{
			X = W * q.X + X * q.W + Y * q.Z - Z * q.Y;
			Y = W * q.Y + Y * q.W + Z * q.X - X * q.Z;
			Z = W * q.Z + Z * q.W + X * q.Y - Y * q.X;
			W = W * q.W - X * q.X - Y * q.Y - Z * q.Z;
			return *this;
		}
		Quaterniont& operator *= (const T& s)
		{
			X *= s;
			Y *= s;
			Z *= s;
			W *= s;
			return *this;
		}
		Quaterniont& operator /= (const T& s)
		{
			return *this *= (1.0 / s);
		}


		T Dot(const Quaterniont& q) const
		{
			return X * q.X + Y * q.Y + Z * q.Z + W * q.W;
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
		Quaterniont Normalized() const
		{
			return *this / Length();
		}

		T AngleBetween(const Quaterniont& q) const
		{
			T s = Sqrt(LengthSquared() * q.LengthSquared());
			if (s == T(0.0)) return T(0.0);
			return ArcCos(Dot(q)/s);
		}

		T Angle() const
		{
			return T(2.0) * ArcCos(W);
		}

		Vector3t<T> Axis() const
		{
			T s2 = T(1.0) - Pow(W, T(2.0));
			if (s2 < T(0.00001)) return Vector3t<T>(T(1.0), T(0.0), T(0.0));
			T s = Sqrt(s2);
			return Vector3t<T>(X / s, Y / s, Z / s);
		}

		Matrix3t<T> ToMatrix3() const
		{
			return Matrix3t<T>::Rotation(Axis(), Angle());
		}
		Matrix4t<T> ToMatrix4() const
		{
			return Matrix4t<T>::Rotation(Axis(), Angle());
		}

		Quaterniont Inverse() const
		{
			return Quaterniont(-X, -Y, -Z, W);
		}

		Quaterniont Slerp(const Quaterniont& q, const T& t) const
		{
			if (t <= T(0.0)) return *this;
			if (t >= T(1.0)) return q;

			T theta = Angle(q);

			if (theta != T(0.0))
			{
				T d = T(1.0) / Sin(theta);
				T s0 = Sin((T(1.0) - t) * theta);
				T s1 = Sin(t * theta);

				Quaterniont r;
				r.X = (X * s0 + q.X * s1) * d;
				r.Y = (Y * s0 + q.Y * s1) * d;
				r.Z = (Z * s0 + q.Z * s1) * d;
				r.W = (W * s0 + q.W * s1) * d;
				return r;
			}

			return *this;
		}

		static const Quaterniont& Identity()
		{
			static const Quaterniont id(0,0,0,1);
			return id;
		}
	};

	/**
		\addtogroup Math
		@{
	*/

	typedef Quaterniont<float> Quaternion;
	typedef Quaterniont<double> Quaterniond;

	/** @} */
}

#endif
