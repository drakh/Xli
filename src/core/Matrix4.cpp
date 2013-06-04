#include <Xli/Matrix4.h>

namespace Xli
{
	float* Matrix4::Data()
	{
		return data;
	}

	const float* Matrix4::Data() const
	{
		return data;
	}

	Matrix4::operator float* ()
	{
		return data;
	}

	Matrix4::operator const float* () const
	{
		return data;
	}

	float& Matrix4::operator [] (int i)
	{
#ifdef XLI_RANGE_CHECK
		if (i >= 16 || i < 0)
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
		return data[i];
	}

	String Matrix4::ToString() const
	{
		String s = data[0];
		for (int i = 1; i < 16; i++) s = s + ", " + data[i];
		return s;
	}

	Matrix4::Matrix4()
	{
	}

	Matrix4::Matrix4(const float* values)
	{
		for (int i = 0; i < 16; i++) data[i] = values[i];
	}

	Matrix4::Matrix4(const Matrix4& m)
	{
		for (int i = 0; i < 16; i++) data[i] = m.data[i];
	}

	Matrix4::Matrix4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15) 
	{ 
		data[0] = v0; data[1] = v1; data[2] = v2; data[3] = v3;
		data[4] = v4; data[5] = v5; data[6] = v6; data[7] = v7;
		data[8] = v8; data[9] = v9; data[10] = v10; data[11] = v11;
		data[12] = v12; data[13] = v13; data[14] = v14; data[15] = v15;
	}

	Matrix4& Matrix4::operator = (const Matrix4& m)
	{
		for (int i = 0; i < 16; i++) data[i] = m.data[i];
		return *this;
	}

	Matrix4 Matrix4::operator + (const Matrix4& m) const
	{
		Matrix4 r;
		for (int i = 0; i < 16; i++) r.data[i] = data[i] + m.data[i];
		return r;
	}

	Matrix4 Matrix4::operator - (const Matrix4& m) const
	{
		Matrix4 r;
		for (int i = 0; i < 16; i++) r.data[i] = data[i] - m.data[i];
		return r;
	}

	Matrix4 Matrix4::operator + (float s) const
	{
		Matrix4 r;
		for (int i = 0; i < 16; i++) r.data[i] = data[i] + s;
		return r;
	}

	Matrix4 Matrix4::operator - (float s) const
	{
		Matrix4 r;
		for (int i = 0; i < 16; i++) r.data[i] = data[i] - s;
		return r;
	}

	Matrix4 Matrix4::operator * (float s) const
	{
		Matrix4 r;
		for (int i = 0; i < 16; i++) r.data[i] = data[i] * s;
		return r;
	}

	Matrix4& Matrix4::operator += (const Matrix4& m)
	{
		for (int i = 0; i < 16; i++) data[i] += m.data[i];
		return *this;
	}

	Matrix4& Matrix4::operator -= (const Matrix4& m)
	{
		for (int i = 0; i < 16; i++) data[i] -= m.data[i];
		return *this;
	}

	Matrix4& Matrix4::operator += (float s)
	{
		for (int i = 0; i < 16; i++) data[i] += s;
		return *this;
	}

	Matrix4& Matrix4::operator -= (float s)
	{
		for (int i = 0; i < 16; i++) data[i] -= s;
		return *this;
	}
	
	Matrix4& Matrix4::operator *= (float s)
	{
		for (int i = 0; i < 16; i++) data[i] *= s;
		return *this;
	}

	Vector3 Matrix4::operator * (const Vector3& v) const
	{
		Vector3 r;
		r.X = data[0]*v.X + data[4]*v.Y + data[8]*v.Z + data[12];
		r.Y = data[1]*v.X + data[5]*v.Y + data[9]*v.Z + data[13];
		r.Z = data[2]*v.X + data[6]*v.Y + data[10]*v.Z + data[14];
		return r;
	}

	Vector4 Matrix4::operator * (const Vector4& v) const
	{
		Vector4 r;
		r.X = data[0]*v.X + data[4]*v.Y + data[8]*v.Z + data[12]*v.W;
		r.Y = data[1]*v.Y + data[5]*v.Y + data[9]*v.Z + data[13]*v.W;
		r.Z = data[2]*v.Z + data[6]*v.Y + data[10]*v.Z + data[14]*v.W;
		r.W = data[3]*v.W + data[7]*v.Y + data[11]*v.Z + data[15]*v.W;
		return r;
	}

	Matrix4 Matrix4::operator * (const Matrix4& m) const
	{
		Matrix4 res;
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

	Matrix4& Matrix4::operator *= (const Matrix4& m)
	{
		return *this = *this * m;
	}


	Matrix4 Matrix4::Transposed() const
	{
		Matrix4 t;
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

	void Matrix4::Transpose()
	{
		*this = Transposed();
	}

	bool Matrix4::Invert()
	{
		Matrix4 inverseMatrix = Matrix4::Identity();

		float* src = data;
		float* inverse = inverseMatrix;

		int i, j, k;
		float t;
		float temp[4][4];

		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				temp[i][j] = src[i*4+j];
			}
		}

		for (i = 0; i < 4; i++)
		{
			if (temp[i][i] == (float)0.0)
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

			t = (float)1.0 / temp[i][i];

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

	Vector3 Matrix4::GetScale() const
	{
		return Vector3(Sqrt((data[0] * data[0]) + (data[1] * data[1]) + (data[2] * data[2])),
			            Sqrt((data[4] * data[4]) + (data[5] * data[5]) + (data[6] * data[6])),
			            Sqrt((data[8] * data[8]) + (data[9] * data[9]) + (data[10] * data[10])));
	}

	Matrix4 Matrix4::Inverse() const
	{
		Matrix4 m = *this;
		m.Invert();
		return m;
	}

	Matrix3 Matrix4::ToMatrix3() const
	{
		Matrix3 m;
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

	const Matrix4& Matrix4::Identity()
	{
		static const float data[16] =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		static const Matrix4 id(data);
		return id;
	}

	Matrix4 Matrix4::Translation(float x, float y, float z)
	{
		Matrix4 m = Identity();
		m.data[12] = x;
		m.data[13] = y;
		m.data[14] = z;
		return m;
	}
	
	Matrix4 Matrix4::Translation(const Vector3& v)
	{
		return Translation(v.X, v.Y, v.Z);
	}

	Matrix4 Matrix4::RotationX(float angleRadians)
	{
		Matrix4 m;
		float* a = m.data;
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		a[ 0] = 1; a[ 4] = 0; a[ 8] = 0; a[12] = 0;
		a[ 1] = 0; a[ 5] = c; a[ 9] =-s; a[13] = 0;
		a[ 2] = 0; a[ 6] = s; a[10] = c; a[14] = 0; 
		a[ 3] = 0; a[ 7] = 0; a[11] = 0; a[15] = 1; 

		return m;
	}
	
	Matrix4 Matrix4::RotationY(float angleRadians)
	{
		Matrix4 m;
		float* a = m.data;
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		a[ 0] = c; a[ 4] = 0; a[ 8] = s; a[12] = 0;
		a[ 1] = 0; a[ 5] = 1; a[ 9] = 0; a[13] = 0;
		a[ 2] =-s; a[ 6] = 0; a[10] = c; a[14] = 0; 
		a[ 3] = 0; a[ 7] = 0; a[11] = 0; a[15] = 1; 

		return m;
	}
	
	Matrix4 Matrix4::RotationZ(float angleRadians)
	{
		Matrix4 m;
		float* a = m.data;
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		a[ 0] = c; a[ 4] =-s; a[ 8] =-0; a[12] = 0;
		a[ 1] = s; a[ 5] = c; a[ 9] = 0; a[13] = 0;
		a[ 2] = 0; a[ 6] = 0; a[10] = 1; a[14] = 0; 
		a[ 3] = 0; a[ 7] = 0; a[11] = 0; a[15] = 1; 

		return m;
	}

	Matrix4 Matrix4::Rotation(const Vector3& axis, float angleRadians)
	{
		Vector3 normalized_axis = Normalize(axis);
		float x = normalized_axis.X;
		float y = normalized_axis.Y;
		float z = normalized_axis.Z;

		Matrix4 m;
		float* a = m.data;
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		float xx = x * x;
		float yy = y * y;
		float zz = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;

		a[ 0] = xx + (c * ((float)1 - xx));
		a[ 1] = (xy - (c * xy)) + (s * z);
		a[ 2] = (xz - (c * xz)) - (s * y);
		a[ 3] = 0;
		a[ 4] = (xy - (c * xy)) - (s * z);
		a[ 5] = yy + (c * ((float)1 - yy));
		a[ 6] = (yz - (c * yz)) + (s * x);
		a[ 7] = 0;
		a[ 8] = (xz - (c * xz)) + (s * y);
		a[ 9] = (yz - (c * yz)) - (s * x);
		a[10] = zz + (c * ((float)1 - zz));
		a[11] = (float)0;
		a[12] = (float)0;
		a[13] = (float)0;
		a[14] = (float)0;
		a[15] = (float)1;
			
		return m;
	}

	Matrix4 Matrix4::Rotation(float x, float y, float z, float angleRadians)
	{
		return Rotation(Vector3(x, y, z), angleRadians);
	}

	Matrix4 Matrix4::Scaling(float x, float y, float z)
	{
		Matrix4 m = Identity();
		m.data[0] = x;
		m.data[5] = y;
		m.data[10] = z;
		return m;
	}

	Matrix4 Matrix4::Scaling(const Vector3& v)
	{
		return Scaling(v.X, v.Y, v.Z);
	}

	Matrix4 Matrix4::Scaling(float s)
	{
		return Scaling(s, s, s);
	}

	Matrix4 Matrix4::GLLookAt(const Vector3& eye, const Vector3& center, const Vector3& upVec)
	{
		Vector3 forward = Normalize(center - eye);

		/* Side = forward x up */
		Vector3 side = Normalize(Cross(forward, upVec));

		/* Recompute up as: up = side x forward */
		Vector3 up = Cross(side, forward);

		Matrix4 m = Identity();
			
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

	Matrix4 Matrix4::GLPerspective(float fovRadians, float aspect, float zNear, float zFar)
	{
		fovRadians = (float)0.5 * fovRadians;

		Matrix4 m = Identity();
		float sine, cotangent, deltaZ;

		deltaZ = zFar - zNear;
		sine = Sin(fovRadians/(float)2);

		if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
			return m;

		cotangent = Cos(fovRadians/(float)2) / sine;

		m.data[ 0] = cotangent / aspect;
		m.data[ 5] = cotangent;
		m.data[10] = -(zFar + zNear) / deltaZ;
		m.data[11] = -1;
		m.data[14] = -2 * zNear * zFar / deltaZ;
		m.data[15] = 0;

		return m;
	}

	Matrix4 Matrix4::GLOrtho(float left, float right, float bottom, float top, float nearval, float farval)
	{
		Matrix4 m;

		#define M(row, col)  m.data[col*4+row]
		M(0,0) = (float)2 / (right-left);
		M(0,1) = (float)0;
		M(0,2) = (float)0;
		M(0,3) = -(right+left) / (right-left);

		M(1,0) = (float)0;
		M(1,1) = (float)2 / (top-bottom);
		M(1,2) = (float)0;
		M(1,3) = -(top+bottom) / (top-bottom);

		M(2,0) = (float)0;
		M(2,1) = (float)0;
		M(2,2) = (float)-2 / (farval-nearval);
		M(2,3) = -(farval+nearval) / (farval-nearval);

		M(3,0) = (float)0;
		M(3,1) = (float)0;
		M(3,2) = (float)0;
		M(3,3) = (float)1;
		#undef M

		return m;
	}
}
