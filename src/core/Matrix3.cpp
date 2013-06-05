#include <Xli/Matrix3.h>

namespace Xli
{
	float* Matrix3::Data()
	{
		return data;
	}

	const float* Matrix3::Data() const
	{
		return data;
	}

	Matrix3::operator float* ()
	{
		return data;
	}

	Matrix3::operator const float* () const
	{
		return data;
	}

	float& Matrix3::operator [] (int i)
	{
#ifdef XLI_RANGE_CHECK
		if (i >= 9 || i < 0)
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
#endif
		return data[i];
	}

	String Matrix3::ToString() const
	{
		String s = data[0];
		for (int i = 1; i < 9; i++) s = s + ", " + data[i];
		return s;
	}

	Matrix3::Matrix3()
	{
	}

	Matrix3::Matrix3(const float* values)
	{
		for (int i = 0; i < 9; i++) data[i] = values[i];
	}

	Matrix3::Matrix3(const Matrix3& m)
	{
		for (int i = 0; i < 9; i++) data[i] = m.data[i];
	}

	Matrix3::Matrix3(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8) 
	{ 
		data[0] = v0; data[1] = v1; data[2] = v2;
		data[3] = v3; data[4] = v4; data[5] = v5;
		data[6] = v6; data[7] = v7; data[8] = v8;
	}

	Matrix3& Matrix3::operator = (const Matrix3& m)
	{
		for (int i = 0; i < 9; i++) data[i] = m.data[i];
		return *this;
	}

	Matrix3 Matrix3::operator + (const Matrix3& m) const
	{
		Matrix3 r;
		for (int i = 0; i < 9; i++) r.data[i] = data[i] + m.data[i];
		return r;
	}

	Matrix3 Matrix3::operator - (const Matrix3& m) const
	{
		Matrix3 r;
		for (int i = 0; i < 9; i++) r.data[i] = data[i] - m.data[i];
		return r;
	}

	Matrix3 Matrix3::operator + (float s) const
	{
		Matrix3 r;
		for (int i = 0; i < 9; i++) r.data[i] = data[i] + s;
		return r;
	}

	Matrix3 Matrix3::operator - (float s) const
	{
		Matrix3 r;
		for (int i = 0; i < 9; i++) r.data[i] = data[i] - s;
		return r;
	}

	Matrix3 Matrix3::operator * (float s) const
	{
		Matrix3 r;
		for (int i = 0; i < 9; i++) r.data[i] = data[i] * s;
		return r;
	}

	Matrix3& Matrix3::operator += (const Matrix3& m)
	{
		for (int i = 0; i < 9; i++) data[i] += m.data[i];
		return *this;
	}

	Matrix3& Matrix3::operator -= (const Matrix3& m)
	{
		for (int i = 0; i < 9; i++) data[i] -= m.data[i];
		return *this;
	}

	Matrix3& Matrix3::operator += (float s)
	{
		for (int i = 0; i < 9; i++) data[i] += s;
		return *this;
	}

	Matrix3& Matrix3::operator -= (float s)
	{
		for (int i = 0; i < 9; i++) data[i] -= s;
		return *this;
	}

	Matrix3& Matrix3::operator *= (float s)
	{
		for (int i = 0; i < 9; i++) data[i] *= s;
		return *this;
	}

	Vector3 Matrix3::operator * (const Vector3& v) const
	{
		Vector3 r;
		r.X = data[0]*v.X + data[3]*v.Y + data[6]*v.Z;
		r.Y = data[1]*v.X + data[4]*v.Y + data[7]*v.Z;
		r.Z = data[2]*v.X + data[5]*v.Y + data[8]*v.Z;
		return r;
	}

	Matrix3 Matrix3::operator * (const Matrix3& m) const
	{
		Matrix3 res;
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

	Matrix3& Matrix3::operator *= (const Matrix3& m)
	{
		return *this = *this * m;
	}

	void Matrix3::Transpose()
	{
		Swap(data[1], data[3]);
		Swap(data[2], data[6]);
		Swap(data[5], data[7]);
	}

	const Matrix3& Matrix3::Identity()
	{
		static const float data[9] = 
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		};

		static const Matrix3 id(data);
		return id;
	}

	Matrix3 Matrix3::RotationX(float angleRadians)
	{
		Matrix3 m;
		float* a = m.data;
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		a[0] = 1; a[3] = 0; a[6] = 0;
		a[1] = 0; a[4] = c; a[7] =-s;
		a[2] = 0; a[5] = s; a[8] = c;

		return m;
	}

	Matrix3 Matrix3::RotationY(float angleRadians)
	{
		Matrix3 m;
		float* a = m.data;
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		a[0] = c; a[3] = 0; a[6] = s;
		a[1] = 0; a[4] = 1; a[7] = 0;
		a[2] =-s; a[5] = 0; a[8] = c;

		return m;
	}

	Matrix3 Matrix3::RotationZ(float angleRadians)
	{
		Matrix3 m;
		float* a = m.data;
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		a[0] = c; a[3] =-s; a[6] =-0;
		a[1] = s; a[4] = c; a[7] = 0;
		a[2] = 0; a[5] = 0; a[8] = 1;

		return m;
	}

	Matrix3 Matrix3::Rotation(const Vector3& axis, float angleRadians)
	{
		Vector3 normalized_axis = Normalize(axis);
		float x = normalized_axis.X;
		float y = normalized_axis.Y;
		float z = normalized_axis.Z;

		Matrix3 m;
		float* a = m.data;
		float c = Cos(angleRadians);
		float s = Sin(angleRadians);

		float xx = x * x;
		float yy = y * y;
		float zz = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;

		a[0] = xx + (c * (1.0f - xx));
		a[1] = (xy - (c * xy)) + (s * z);
		a[2] = (xz - (c * xz)) - (s * y);
		a[3] = (xy - (c * xy)) - (s * z);
		a[4] = yy + (c * (1.0f - yy));
		a[5] = (yz - (c * yz)) + (s * x);
		a[6] = (xz - (c * xz)) + (s * y);
		a[7] = (yz - (c * yz)) - (s * x);
		a[8] = zz + (c * (1.0f - zz));
			
		return m;
	}

	Matrix3 Matrix3::Rotation(float x, float y, float z, float angleRadians)
	{
		return Rotation(Vector3(x, y, z), angleRadians);
	}

	Matrix3 Matrix3::Scaling(float x, float y, float z)
	{
		Matrix3 m = Identity();
		m.data[0] = x;
		m.data[4] = y;
		m.data[8] = z;
		return m;
	}

	Matrix3 Matrix3::Scaling(const Vector3& v)
	{
		return Scaling(v.X, v.Y, v.Z);
	}

	Matrix3 Matrix3::Scaling(float s)
	{
		return Scaling(s, s, s);
	}
}
