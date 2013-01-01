#ifndef __XLI_MATRIX3_H__
#define __XLI_MATRIX3_H__

#include "Math.h"
#include "Utils.h"

namespace Xli
{
	/**
		\ingroup Math
	*/
	template <typename T> class Matrix3t
	{
		T data[9];

	public:
		T* Data()
		{
			return data;
		}
		const T* Data() const
		{
			return data;
		}
		operator T* ()
		{
			return data;
		}
		operator const T* () const
		{
			return data;
		}
		T& operator [] (int i)
		{
#ifdef XLI_RANGE_CHECK
			if (i >= 9 || i < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			return data[i];
		}
		String ToString() const
		{
			String s = data[0];
			for (int i = 1; i < 9; i++) s = s + ", " + data[i];
			return s;
		}

		Matrix3t()
		{
		}
		explicit Matrix3t(const T* values)
		{
			for (int i = 0; i < 9; i++) data[i] = values[i];
		}
		Matrix3t(const Matrix3t& m)
		{
			for (int i = 0; i < 9; i++) data[i] = m.data[i];
		}
		Matrix3t(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8) 
		{ 
			data[0] = v0; data[1] = v1; data[2] = v2;
			data[3] = v3; data[4] = v4; data[5] = v5;
			data[6] = v6; data[7] = v7; data[8] = v8;
		}
		Matrix3t& operator = (const Matrix3t& m)
		{
			for (int i = 0; i < 9; i++) data[i] = m.data[i];
			return *this;
		}

		Matrix3t operator + (const Matrix3t& m) const
		{
			Matrix3t r;
			for (int i = 0; i < 9; i++) r.data[i] = data[i] + m.data[i];
			return r;
		}
		Matrix3t operator - (const Matrix3t& m) const
		{
			Matrix3t r;
			for (int i = 0; i < 9; i++) r.data[i] = data[i] - m.data[i];
			return r;
		}

		Matrix3t operator + (const T& s) const
		{
			Matrix3t r;
			for (int i = 0; i < 9; i++) r.data[i] = data[i] + s;
			return r;
		}
		Matrix3t operator - (const T& s) const
		{
			Matrix3t r;
			for (int i = 0; i < 9; i++) r.data[i] = data[i] - s;
			return r;
		}
		Matrix3t operator * (const T& s) const
		{
			Matrix3t r;
			for (int i = 0; i < 9; i++) r.data[i] = data[i] * s;
			return r;
		}


		Matrix3t& operator += (const Matrix3t& m) const
		{
			for (int i = 0; i < 9; i++) data[i] += m.data[i];
			return *this;
		}
		Matrix3t& operator -= (const Matrix3t& m) const
		{
			for (int i = 0; i < 9; i++) data[i] -= m.data[i];
			return *this;
		}

		Matrix3t& operator += (const T& s) const
		{
			for (int i = 0; i < 9; i++) data[i] += s;
			return *this;
		}
		Matrix3t& operator -= (const T& s) const
		{
			for (int i = 0; i < 9; i++) data[i] -= s;
			return *this;
		}
		Matrix3t& operator *= (const T& s) const
		{
			for (int i = 0; i < 9; i++) data[i] *= s;
			return *this;
		}

		Vector3t<T> operator * (const Vector3t<T>& v) const
		{
			Vector3t<T> r;
			r.X = data[0]*v.X + data[3]*v.Y + data[6]*v.Z;
			r.Y = data[1]*v.X + data[4]*v.Y + data[7]*v.Z;
			r.Z = data[2]*v.X + data[5]*v.Y + data[8]*v.Z;
			return r;
		}

		Matrix3t operator * (const Matrix3t& m) const
		{
			Matrix3t res;
			for(int i = 0; i < 3; ++i)
			{
				int idx = i*3;
				for(int j = 0; j < 3; ++j)
				{
					res.data[idx+j] = 
						m.data[idx+0] * data[0*3+j] +
						m.data[idx+1] * data[1*3+j] +
						m.data[idx+2] * data[2*3+j];
				}
			}
			return res;
		}

		Matrix3t& operator *= (const Matrix3t& m)
		{
			return *this = *this * m;
		}

		void Transpose()
		{
			Swap(data[1], data[3]);
			Swap(data[2], data[6]);
			Swap(data[5], data[7]);
		}

		static const Matrix3t& Identity()
		{
			static const T data[9] = 
			{
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			};

			static const Matrix3t id(data);
			return id;
		}

		static Matrix3t RotationX(const T& angleRadians)
		{
			Matrix3t m;
			T* a = m.data;
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);

			a[0] = 1; a[3] = 0; a[6] = 0;
			a[1] = 0; a[4] = c; a[7] =-s;
			a[2] = 0; a[5] = s; a[8] = c;

			return m;
		}
		static Matrix3t RotationY(const T& angleRadians)
		{
			Matrix3t m;
			T* a = m.data;
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);

			a[0] = c; a[3] = 0; a[6] = s;
			a[1] = 0; a[4] = 1; a[7] = 0;
			a[2] =-s; a[5] = 0; a[8] = c;

			return m;
		}
		static Matrix3t RotationZ(const T& angleRadians)
		{
			Matrix3t m;
			T* a = m.data;
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);

			a[0] = c; a[3] =-s; a[6] =-0;
			a[1] = s; a[4] = c; a[7] = 0;
			a[2] = 0; a[5] = 0; a[8] = 1;

			return m;
		}

		static Matrix3t Rotation(const Vector3t<T>& axis, const T& angleRadians)
		{
			Vector3t<T> normalized_axis = axis.Normalized();
			T& x = normalized_axis.X;
			T& y = normalized_axis.Y;
			T& z = normalized_axis.Z;

			Matrix3t m;
			T* a = m.data;
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);

			float xx = x * x;
			float yy = y * y;
			float zz = z * z;
			float xy = x * y;
			float xz = x * z;
			float yz = y * z;

			a[0] = xx + (c * ((T)1 - xx));
			a[1] = (xy - (c * xy)) + (s * z);
			a[2] = (xz - (c * xz)) - (s * y);
			a[3] = (xy - (c * xy)) - (s * z);
			a[4] = yy + (c * ((T)1 - yy));
			a[5] = (yz - (c * yz)) + (s * x);
			a[6] = (xz - (c * xz)) + (s * y);
			a[7] = (yz - (c * yz)) - (s * x);
			a[8] = zz + (c * ((T)1 - zz));
			
			return m;
		}
		static Matrix3t Rotation(const T& x, const T& y, const T& z, const T& angleRadians)
		{
			return Rotation(Vector3t<T>(x, y, z), angleRadians);
		}

		static Matrix3t Scaling(const T& x, const T& y, const T& z)
		{
			Matrix3t m = Identity();
			m.data[0] = x;
			m.data[4] = y;
			m.data[9] = z;
			return m;
		}
		static Matrix3t Scaling(const Vector3t<T>& v)
		{
			return Scaling(v.X, v.Y, v.Z);
		}
		static Matrix3t Scaling(const T& s)
		{
			return Scaling(s, s, s);
		}
	};

	/**
		\addtogroup Math
		@{
	*/

	typedef Matrix3t<float> Matrix3;
	typedef Matrix3t<double> Matrix3d;

	/** @} */
}

#endif
