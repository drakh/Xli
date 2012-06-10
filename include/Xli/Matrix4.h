#ifndef __XLI_MATRIX4_H__
#define __XLI_MATRIX4_H__

#include "Vector4.h"
#include "Matrix3.h"

namespace Xli
{
	template <typename T> class Matrix4t
	{
		T data[16];

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
			if (i >= 16 || i < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			return data[i];
		}
		String ToString() const
		{
			String s = data[0];
			for (int i = 1; i < 16; i++) s = s + ", " + data[i];
			return s;
		}

		Matrix4t()
		{
		}
		explicit Matrix4t(const T* values)
		{
			for (int i = 0; i < 16; i++) data[i] = values[i];
		}
		Matrix4t(const Matrix4t& m)
		{
			for (int i = 0; i < 16; i++) data[i] = m.data[i];
		}
		Matrix4t(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15) 
		{ 
			data[0] = v0; data[1] = v1; data[2] = v2; data[3] = v3;
			data[4] = v4; data[5] = v5; data[6] = v6; data[7] = v7;
			data[8] = v8; data[9] = v9; data[10] = v10; data[11] = v11;
			data[12] = v12; data[13] = v13; data[14] = v14; data[15] = v15;
		}
		Matrix4t& operator = (const Matrix4t& m)
		{
			for (int i = 0; i < 16; i++) data[i] = m.data[i];
			return *this;
		}

		Matrix4t operator + (const Matrix4t& m) const
		{
			Matrix4t r;
			for (int i = 0; i < 16; i++) r.data[i] = data[i] + m.data[i];
			return r;
		}
		Matrix4t operator - (const Matrix4t& m) const
		{
			Matrix4t r;
			for (int i = 0; i < 16; i++) r.data[i] = data[i] - m.data[i];
			return r;
		}

		Matrix4t operator + (const T& s) const
		{
			Matrix4t r;
			for (int i = 0; i < 16; i++) r.data[i] = data[i] + s;
			return r;
		}
		Matrix4t operator - (const T& s) const
		{
			Matrix4t r;
			for (int i = 0; i < 16; i++) r.data[i] = data[i] - s;
			return r;
		}
		Matrix4t operator * (const T& s) const
		{
			Matrix4t r;
			for (int i = 0; i < 16; i++) r.data[i] = data[i] * s;
			return r;
		}


		Matrix4t& operator += (const Matrix4t& m) const
		{
			for (int i = 0; i < 16; i++) data[i] += m.data[i];
			return *this;
		}
		Matrix4t& operator -= (const Matrix4t& m) const
		{
			for (int i = 0; i < 16; i++) data[i] -= m.data[i];
			return *this;
		}

		Matrix4t& operator += (const T& s) const
		{
			for (int i = 0; i < 16; i++) data[i] += s;
			return *this;
		}
		Matrix4t& operator -= (const T& s) const
		{
			for (int i = 0; i < 16; i++) data[i] -= s;
			return *this;
		}
		Matrix4t& operator *= (const T& s) const
		{
			for (int i = 0; i < 16; i++) data[i] *= s;
			return *this;
		}


		Vector3t<T> operator * (const Vector3t<T>& v) const
		{
			Vector3t<T> r;
			r.X = data[0]*v.X + data[4]*v.Y + data[8]*v.Z + data[12];
			r.Y = data[1]*v.X + data[5]*v.Y + data[9]*v.Z + data[13];
			r.Z = data[2]*v.X + data[6]*v.Y + data[10]*v.Z + data[14];
			return r;
		}

		Vector4t<T> operator * (const Vector4t<T>& v) const
		{
			Vector4t<T> r;
			r.X = data[0]*v.X + data[4]*v.Y + data[8]*v.Z + data[12]*v.W;
			r.Y = data[1]*v.Y + data[5]*v.Y + data[9]*v.Z + data[13]*v.W;
			r.Z = data[2]*v.Z + data[6]*v.Y + data[10]*v.Z + data[14]*v.W;
			r.W = data[3]*v.W + data[7]*v.Y + data[11]*v.Z + data[15]*v.W;
			return r;
		}

		Matrix4t operator * (const Matrix4t& m) const
		{
			Matrix4t res;
			for (int i = 0; i < 4; ++i)
			{
				int idx = i*4;
				for (int j = 0; j < 4; ++j)
				{
					res.data[idx+j] =
						m.data[idx+0] * data[0*4+j] +
						m.data[idx+1] * data[1*4+j] +
						m.data[idx+2] * data[2*4+j] +
						m.data[idx+3] * data[3*4+j];
				}
			}
			return res;
		}

		Matrix4t& operator *= (const Matrix4t& m)
		{
			return *this = *this * m;
		}


		Matrix4t Transposed() const
		{
			Matrix4t t;
			t.data[0] = data[0];
			t.data[1] = data[4];
			t.data[2] = data[8];
			t.data[3] = data[12];
			t.data[4] = data[1];
			t.data[5] = data[5];
			t.data[6] = data[9];
			t.data[7] = data[13];
			t.data[8] = data[2];
			t.data[9] = data[6];
			t.data[10] = data[10];
			t.data[11] = data[14];
			t.data[12] = data[3];
			t.data[13] = data[7];
			t.data[14] = data[11];
			t.data[15] = data[15];
			return t;
		}
		void Transpose()
		{
			*this = Transposed();
		}

		bool Invert()
		{
			Matrix4t inverseMatrix = Matrix4t::Identity();

			T* src = data;
			T* inverse = inverseMatrix;

			int i, j, k;
			T t;
			T temp[4][4];

			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					temp[i][j] = src[i*4+j];
				}
			}

			for (i = 0; i < 4; i++)
			{
				if (temp[i][i] == (T)0.0)
				{
					for (j = i + 1; j < 4; j++)
						if (temp[j][i] != 0.0) break;

					if (j != 4)
					{
						for (k = 0; k < 4; k++)
						{
							t = temp[i][k];
							temp[i][k] = temp[j][k];
							temp[j][k] = t;

							t = inverse[i*4+k];
							inverse[i*4+k] = inverse[j*4+k];
							inverse[j*4+k] = t;
						}
					}
					else
					{
						return false;
					}
				}

				t = (T)1.0 / temp[i][i];

				for (k = 0; k < 4; k++)
				{
					temp[i][k] *= t;
					inverse[i*4+k] *= t;
				}
				for (j = 0; j < 4; j++)
				{
					if (j != i)
					{
						t = temp[j][i];
						for (k = 0; k < 4; k++)
						{
							temp[j][k] -= temp[i][k]*t;
							inverse[j*4+k] -= inverse[i*4+k]*t;
						}
					}
				}
			}

			for (int i = 0; i < 16; i++) src[i] = inverse[i];
			return true;
		}

		Vector3 GetScale() const
		{
			return Vector3(Sqrt((data[0] * data[0]) + (data[1] * data[1]) + (data[2] * data[2])),
			               Sqrt((data[4] * data[4]) + (data[5] * data[5]) + (data[6] * data[6])),
			               Sqrt((data[8] * data[8]) + (data[9] * data[9]) + (data[10] * data[10])));
		}

		float GetScaleMax() const
		{
			Vector3 s = GetScale();
			return Max(Max(s.X, s.Y), s.Z);
		}

		Matrix4t<T> Inverse() const
		{
			Matrix4t<T> m = *this;
			m.Invert();
			return m;
		}

		Matrix3t<T> ToMatrix3() const
		{
			Matrix3t<T> m;
			m[0] = data[0];
			m[1] = data[1];
			m[2] = data[2];
			m[3] = data[4];
			m[4] = data[5];
			m[5] = data[6];
			m[6] = data[8];
			m[7] = data[9];
			m[8] = data[10];
			return m;
		}

		static const Matrix4t& Identity()
		{
			static const T data[16] =
			{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};

			static const Matrix4t id(data);
			return id;
		}

		static Matrix4t Translation(const T& x, const T& y, const T& z)
		{
			Matrix4t m = Identity();
			m.data[12] = x;
			m.data[13] = y;
			m.data[14] = z;
			return m;
		}
		static Matrix4t Translation(const Vector3t<T>& v)
		{
			return Translation(v.X, v.Y, v.Z);
		}

		static Matrix4t RotationX(const T& angleRadians)
		{
			Matrix4t m;
			T* a = m.data;
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);

			a[ 0] = 1; a[ 4] = 0; a[ 8] = 0; a[12] = 0;
			a[ 1] = 0; a[ 5] = c; a[ 9] =-s; a[13] = 0;
			a[ 2] = 0; a[ 6] = s; a[10] = c; a[14] = 0; 
			a[ 3] = 0; a[ 7] = 0; a[11] = 0; a[15] = 1; 

			return m;
		}
		static Matrix4t RotationY(const T& angleRadians)
		{
			Matrix4t m;
			T* a = m.data;
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);

			a[ 0] = c; a[ 4] = 0; a[ 8] = s; a[12] = 0;
			a[ 1] = 0; a[ 5] = 1; a[ 9] = 0; a[13] = 0;
			a[ 2] =-s; a[ 6] = 0; a[10] = c; a[14] = 0; 
			a[ 3] = 0; a[ 7] = 0; a[11] = 0; a[15] = 1; 

			return m;
		}
		static Matrix4t RotationZ(const T& angleRadians)
		{
			Matrix4t m;
			T* a = m.data;
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);

			a[ 0] = c; a[ 4] =-s; a[ 8] =-0; a[12] = 0;
			a[ 1] = s; a[ 5] = c; a[ 9] = 0; a[13] = 0;
			a[ 2] = 0; a[ 6] = 0; a[10] = 1; a[14] = 0; 
			a[ 3] = 0; a[ 7] = 0; a[11] = 0; a[15] = 1; 

			return m;
		}

		static Matrix4t Rotation(const Vector3t<T>& axis, const T& angleRadians)
		{
			Vector3t<T> normalized_axis = axis.Normalized();
			T& x = normalized_axis.X;
			T& y = normalized_axis.Y;
			T& z = normalized_axis.Z;

			Matrix4t m;
			T* a = m.data;
			T c = Cos(angleRadians);
			T s = Sin(angleRadians);

			T xx = x * x;
			T yy = y * y;
			T zz = z * z;
			T xy = x * y;
			T xz = x * z;
			T yz = y * z;

			a[ 0] = xx + (c * ((T)1 - xx));
			a[ 1] = (xy - (c * xy)) + (s * z);
			a[ 2] = (xz - (c * xz)) - (s * y);
			a[ 3] = 0;
			a[ 4] = (xy - (c * xy)) - (s * z);
			a[ 5] = yy + (c * ((T)1 - yy));
			a[ 6] = (yz - (c * yz)) + (s * x);
			a[ 7] = 0;
			a[ 8] = (xz - (c * xz)) + (s * y);
			a[ 9] = (yz - (c * yz)) - (s * x);
			a[10] = zz + (c * ((T)1 - zz));
			a[11] = (T)0;
			a[12] = (T)0;
			a[13] = (T)0;
			a[14] = (T)0;
			a[15] = (T)1;
			
			return m;
		}
		static Matrix4t Rotation(const T& x, const T& y, const T& z, const T& angleRadians)
		{
			return Rotation(Vector3t<T>(x, y, z), angleRadians);
		}

		static Matrix4t Scaling(const T& x, const T& y, const T& z)
		{
			Matrix4t m = Identity();
			m.data[0] = x;
			m.data[5] = y;
			m.data[10] = z;
			return m;
		}
		static Matrix4t Scaling(const Vector3t<T>& v)
		{
			return Scaling(v.X, v.Y, v.Z);
		}
		static Matrix4t Scaling(const T& s)
		{
			return Scaling(s, s, s);
		}

		static Matrix4t LookAtGL(Vector3t<T> eye, Vector3t<T> center, Vector3t<T> up)
		{
			Vector3t<T> forward = (center - eye).Normalized();

			/* Side = forward x up */
			Vector3t<T> side = forward.Cross(up).Normalized();

			/* Recompute up as: up = side x forward */
			up = side.Cross(forward);

			Matrix4t m = Identity();
			
			m[ 0] = side[0];
			m[ 4] = side[1];
			m[ 8] = side[2];

			m[ 1] = up[0];
			m[ 5] = up[1];
			m[ 9] = up[2];

			m[ 2] = -forward[0];
			m[ 6] = -forward[1];
			m[10] = -forward[2];

			return m * Translation(-eye);
		}

		static Matrix4t PerspectiveGL(T fovRadians, T aspect, T zNear, T zFar)
		{
			fovRadians = (T)0.5 * fovRadians;

			Matrix4t m = Identity();
			T sine, cotangent, deltaZ;

			deltaZ = zFar - zNear;
			sine = Sin(fovRadians/(T)2);

			if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
			{
				return m;
			}

			cotangent = Cos(fovRadians/(T)2) / sine;

			m.data[ 0] = cotangent / aspect;
			m.data[ 5] = cotangent;
			m.data[10] = -(zFar + zNear) / deltaZ;
			m.data[11] = -1;
			m.data[14] = -2 * zNear * zFar / deltaZ;
			m.data[15] = 0;

			return m;
		}

		static Matrix4t OrthoGL(T left, T right, T bottom, T top, T nearval = (T)-1, T farval = (T)1)
		{
		    Matrix4t m;

			#define M(row, col)  m.data[col*4+row]
			M(0,0) = (T)2 / (right-left);
			M(0,1) = (T)0;
			M(0,2) = (T)0;
			M(0,3) = -(right+left) / (right-left);

			M(1,0) = (T)0;
			M(1,1) = (T)2 / (top-bottom);
			M(1,2) = (T)0;
			M(1,3) = -(top+bottom) / (top-bottom);

			M(2,0) = (T)0;
			M(2,1) = (T)0;
			M(2,2) = (T)-2 / (farval-nearval);
			M(2,3) = -(farval+nearval) / (farval-nearval);

			M(3,0) = (T)0;
			M(3,1) = (T)0;
			M(3,2) = (T)0;
			M(3,3) = (T)1;
			#undef M

		    return m;
		}
	};

	typedef Matrix4t<float> Matrix4;
	typedef Matrix4t<double> Matrix4d;
}

#endif
