#include <Xli/Sort.h>
#include <Xli/Matrix3.h>

namespace Xli
{
    Matrix3::operator float* ()
    {
        return Data;
    }

    Matrix3::operator const float* () const
    {
        return Data;
    }

    String Matrix3::ToString() const
    {
        String s = Data[0];

        for (int i = 1; i < 9; i++) 
            s = s + ", " + Data[i];

        return s;
    }

    Quaternion Matrix3::ToQuaternion() const
    {
        float sqrt;
        float halff;
        float scale = Data[0*3 + 0] + Data[1*3 + 1] + Data[2*3 + 2];

        Quaternion result;

        if (scale > 0.0f)
        {
            sqrt = Sqrt(scale + 1.0f);
            result.W = sqrt * 0.5f;
            sqrt = 0.5f / sqrt;

            result.X = (Data[1*3 + 2] - Data[2*3 + 1]) * sqrt;
            result.Y = (Data[2*3 + 0] - Data[0*3 + 2]) * sqrt;
            result.Z = (Data[0*3 + 1] - Data[1*3 + 0]) * sqrt;
        }
        else if ((Data[0*3 + 0] >= Data[1*3 + 1]) && (Data[0*3 + 0] >= Data[2*3 + 2]))
        {
            sqrt = Sqrt(1.0f + Data[0*3 + 0] - Data[1*3 + 1] - Data[2*3 + 2]);
            halff = 0.5f / sqrt;

            result.X = 0.5f * sqrt;
            result.Y = (Data[0*3 + 1] + Data[1*3 + 0]) * halff;
            result.Z = (Data[0*3 + 2] + Data[2*3 + 0]) * halff;
            result.W = (Data[1*3 + 2] - Data[2*3 + 1]) * halff;
        }
        else if (Data[1*3 + 1] > Data[2*3 + 2])
        {
            sqrt = Sqrt(1.0f + Data[1*3 + 1] - Data[0*3 + 0] - Data[2*3 + 2]);
            halff = 0.5f / sqrt;

            result.X = (Data[1*3 + 0] + Data[0*3 + 1]) * halff;
            result.Y = 0.5f * sqrt;
            result.Z = (Data[2*3 + 1] + Data[1*3 + 2]) * halff;
            result.W = (Data[2*3 + 0] - Data[0*3 + 2]) * halff;
        }
        else
        {
            sqrt = Sqrt(1.0f + Data[2*3 + 2] - Data[0*3 + 0] - Data[1*3 + 1]);
            halff = 0.5f / sqrt;

            result.X = (Data[2*3 + 0] + Data[0*3 + 2]) * halff;
            result.Y = (Data[2*3 + 1] + Data[1*3 + 2]) * halff;
            result.Z = 0.5f * sqrt;
            result.W = (Data[0*3 + 1] - Data[1*3 + 0]) * halff;
        }

        return result;
    }

    Matrix3::Matrix3()
    {
    }

    Matrix3::Matrix3(const float* values)
    {
        for (int i = 0; i < 9; i++) 
            Data[i] = values[i];
    }

    Matrix3::Matrix3(const Matrix3& m)
    {
        for (int i = 0; i < 9; i++) 
            Data[i] = m.Data[i];
    }

    Matrix3::Matrix3(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8) 
    { 
        Data[0] = v0; Data[1] = v1; Data[2] = v2;
        Data[3] = v3; Data[4] = v4; Data[5] = v5;
        Data[6] = v6; Data[7] = v7; Data[8] = v8;
    }

    Matrix3& Matrix3::operator = (const Matrix3& m)
    {
        for (int i = 0; i < 9; i++) 
            Data[i] = m.Data[i];

        return *this;
    }

    Matrix3 Matrix3::operator + (const Matrix3& m) const
    {
        Matrix3 r;

        for (int i = 0; i < 9; i++) 
            r.Data[i] = Data[i] + m.Data[i];

        return r;
    }

    Matrix3 Matrix3::operator - (const Matrix3& m) const
    {
        Matrix3 r;

        for (int i = 0; i < 9; i++) 
            r.Data[i] = Data[i] - m.Data[i];

        return r;
    }

    Matrix3 Matrix3::operator + (float s) const
    {
        Matrix3 r;

        for (int i = 0; i < 9; i++) 
            r.Data[i] = Data[i] + s;

        return r;
    }

    Matrix3 Matrix3::operator - (float s) const
    {
        Matrix3 r;

        for (int i = 0; i < 9; i++) 
            r.Data[i] = Data[i] - s;

        return r;
    }

    Matrix3 Matrix3::operator * (float s) const
    {
        Matrix3 r;

        for (int i = 0; i < 9; i++) 
            r.Data[i] = Data[i] * s;

        return r;
    }

    Matrix3& Matrix3::operator += (const Matrix3& m)
    {
        for (int i = 0; i < 9; i++) 
            Data[i] += m.Data[i];

        return *this;
    }

    Matrix3& Matrix3::operator -= (const Matrix3& m)
    {
        for (int i = 0; i < 9; i++) 
            Data[i] -= m.Data[i];

        return *this;
    }

    Matrix3& Matrix3::operator += (float s)
    {
        for (int i = 0; i < 9; i++)
            Data[i] += s;

        return *this;
    }

    Matrix3& Matrix3::operator -= (float s)
    {
        for (int i = 0; i < 9; i++) 
            Data[i] -= s;

        return *this;
    }

    Matrix3& Matrix3::operator *= (float s)
    {
        for (int i = 0; i < 9; i++) 
            Data[i] *= s;

        return *this;
    }

    Vector3 Matrix3::operator * (const Vector3& v) const
    {
        Vector3 r;
        r.X = Data[0]*v.X + Data[3]*v.Y + Data[6]*v.Z;
        r.Y = Data[1]*v.X + Data[4]*v.Y + Data[7]*v.Z;
        r.Z = Data[2]*v.X + Data[5]*v.Y + Data[8]*v.Z;
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
                res.Data[idx+j] = 
                    m.Data[idx+0] * Data[0*3+j] +
                    m.Data[idx+1] * Data[1*3+j] +
                    m.Data[idx+2] * Data[2*3+j];
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
        Swap(Data[1], Data[3]);
        Swap(Data[2], Data[6]);
        Swap(Data[5], Data[7]);
    }

    const Matrix3& Matrix3::Identity()
    {
        static const float Data[9] = 
        {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        };

        static const Matrix3 id(Data);
        return id;
    }

    Matrix3 Matrix3::RotationX(float angleRadians)
    {
        Matrix3 m;
        float* a = m.Data;
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
        float* a = m.Data;
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
        float* a = m.Data;
        float c = Cos(angleRadians);
        float s = Sin(angleRadians);

        a[0] = c; a[3] =-s; a[6] =-0;
        a[1] = s; a[4] = c; a[7] = 0;
        a[2] = 0; a[5] = 0; a[8] = 1;

        return m;
    }

    Matrix3 Matrix3::Rotation(const Vector3& axis, float angleRadians)
    {
        float len = Length(axis);

        if (Abs(len) < FloatZeroTolerance) 
            return Identity();

        float lenInv = 1.0f / len;

        float x = axis.X * lenInv;
        float y = axis.Y * lenInv;
        float z = axis.Z * lenInv;

        Matrix3 m;
        float* a = m.Data;
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

    Matrix3 Matrix3::Rotation(const Quaternion& q)
    {
        return Rotation(q.Axis(), q.Angle());
    }

    Matrix3 Matrix3::Scaling(float x, float y, float z)
    {
        Matrix3 m = Identity();
        m.Data[0] = x;
        m.Data[4] = y;
        m.Data[8] = z;
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
