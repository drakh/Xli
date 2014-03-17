#include <Xli/Matrix4.h>

namespace Xli
{
    Matrix4::operator float* ()
    {
        return Data;
    }

    Matrix4::operator const float* () const
    {
        return Data;
    }

    String Matrix4::ToString() const
    {
        String s = Data[0];
        
        for (int i = 1; i < 16; i++) 
            s = s + ", " + Data[i];

        return s;
    }

    Matrix4::Matrix4()
    {
    }

    Matrix4::Matrix4(const float* values)
    {
        for (int i = 0; i < 16; i++) 
            Data[i] = values[i];
    }

    Matrix4::Matrix4(const Matrix4& m)
    {
        for (int i = 0; i < 16; i++) 
            Data[i] = m.Data[i];
    }

    Matrix4::Matrix4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15) 
    { 
        Data[0] = v0; Data[1] = v1; Data[2] = v2; Data[3] = v3;
        Data[4] = v4; Data[5] = v5; Data[6] = v6; Data[7] = v7;
        Data[8] = v8; Data[9] = v9; Data[10] = v10; Data[11] = v11;
        Data[12] = v12; Data[13] = v13; Data[14] = v14; Data[15] = v15;
    }

    Matrix4& Matrix4::operator = (const Matrix4& m)
    {
        for (int i = 0; i < 16; i++) 
            Data[i] = m.Data[i];

        return *this;
    }

    Matrix4 Matrix4::operator + (const Matrix4& m) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++)
            r.Data[i] = Data[i] + m.Data[i];

        return r;
    }

    Matrix4 Matrix4::operator - (const Matrix4& m) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++) 
            r.Data[i] = Data[i] - m.Data[i];

        return r;
    }

    Matrix4 Matrix4::operator + (float s) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++) 
            r.Data[i] = Data[i] + s;

        return r;
    }

    Matrix4 Matrix4::operator - (float s) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++) 
            r.Data[i] = Data[i] - s;

        return r;
    }

    Matrix4 Matrix4::operator * (float s) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++) 
            r.Data[i] = Data[i] * s;

        return r;
    }

    Matrix4& Matrix4::operator += (const Matrix4& m)
    {
        for (int i = 0; i < 16; i++) 
            Data[i] += m.Data[i];

        return *this;
    }

    Matrix4& Matrix4::operator -= (const Matrix4& m)
    {
        for (int i = 0; i < 16; i++) 
            Data[i] -= m.Data[i];

        return *this;
    }

    Matrix4& Matrix4::operator += (float s)
    {
        for (int i = 0; i < 16; i++) 
            Data[i] += s;

        return *this;
    }

    Matrix4& Matrix4::operator -= (float s)
    {
        for (int i = 0; i < 16; i++) 
            Data[i] -= s;

        return *this;
    }
    
    Matrix4& Matrix4::operator *= (float s)
    {
        for (int i = 0; i < 16; i++) 
            Data[i] *= s;

        return *this;
    }

    Vector3 Matrix4::operator * (const Vector3& v) const
    {
        Vector3 r;
        r.X = Data[0] * v.X + Data[4] * v.Y + Data[8] * v.Z + Data[12];
        r.Y = Data[1] * v.X + Data[5] * v.Y + Data[9] * v.Z + Data[13];
        r.Z = Data[2] * v.X + Data[6] * v.Y + Data[10] * v.Z + Data[14];
        return r;
    }

    Vector4 Matrix4::operator * (const Vector4& v) const
    {
        Vector4 r;
        r.X = Data[0] * v.X + Data[4] * v.Y + Data[8] * v.Z + Data[12] * v.W;
        r.Y = Data[1] * v.X + Data[5] * v.Y + Data[9] * v.Z + Data[13] * v.W;
        r.Z = Data[2] * v.X + Data[6] * v.Y + Data[10] * v.Z + Data[14] * v.W;
        r.W = Data[3] * v.X + Data[7] * v.Y + Data[11] * v.Z + Data[15] * v.W;
        return r;
    }

    Matrix4 Matrix4::operator * (const Matrix4& m) const
    {
        Matrix4 res;

        for (int i = 0; i < 4; ++i)
        {
            int idx = i * 4;
            for (int j = 0; j < 4; ++j)
            {
                res.Data[idx+j] =
                    m.Data[idx + 0] * Data[0 * 4 + j] +
                    m.Data[idx + 1] * Data[1 * 4 + j] +
                    m.Data[idx + 2] * Data[2 * 4 + j] +
                    m.Data[idx + 3] * Data[3 * 4 + j];
            }
        }

        return res;
    }

    Matrix4& Matrix4::operator *= (const Matrix4& m)
    {
        return *this = *this * m;
    }

    bool Matrix4::Decompose(const Matrix4& m, Quaternion& outRotation, Vector3& outScale, Vector3& outTranslation)
    {
        outTranslation = m.GetTranslation();
        outScale = m.GetScaling();

        if (Abs(outScale.X) < FloatZeroTolerance ||
            Abs(outScale.Y) < FloatZeroTolerance ||
            Abs(outScale.Z) < FloatZeroTolerance)
        {
            outRotation = Quaternion::Identity();
            return false;
        }

        Matrix3 rotationMatrix = Matrix3(
            m[0*4 + 0] / outScale.X, m[0*4 + 1] / outScale.X, m[0*4 + 2] / outScale.X,
            m[1*4 + 0] / outScale.Y, m[1*4 + 1] / outScale.Y, m[1*4 + 2] / outScale.Y,
            m[2*4 + 0] / outScale.Z, m[2*4 + 1] / outScale.Z, m[2*4 + 2] / outScale.Z);

        outRotation = rotationMatrix.ToQuaternion();
        return true;
    }

    Matrix4 Matrix4::Transpose(const Matrix4& m)
    {
        Matrix4 t;
        t.Data[0] = m.Data[0];
        t.Data[1] = m.Data[4];
        t.Data[2] = m.Data[8];
        t.Data[3] = m.Data[12];
        t.Data[4] = m.Data[1];
        t.Data[5] = m.Data[5];
        t.Data[6] = m.Data[9];
        t.Data[7] = m.Data[13];
        t.Data[8] = m.Data[2];
        t.Data[9] = m.Data[6];
        t.Data[10] = m.Data[10];
        t.Data[11] = m.Data[14];
        t.Data[12] = m.Data[3];
        t.Data[13] = m.Data[7];
        t.Data[14] = m.Data[11];
        t.Data[15] = m.Data[15];
        return t;
    }

    void Matrix4::Transpose()
    {
        *this = Transpose(*this);
    }

    bool Matrix4::Invert()
    {
        Matrix4 inverseMatrix = Matrix4::Identity();

        float* src = Data;
        float* inverse = inverseMatrix;

        int i, j, k;
        float t;
        float temp[4][4];

        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                temp[i][j] = src[i*4+j];

        for (i = 0; i < 4; i++)
        {
            if (temp[i][i] == (float)0.0)
            {
                for (j = i + 1; j < 4; j++)
                    if (temp[j][i] != 0.0) 
                        break;

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

        for (int i = 0; i < 16; i++) 
            src[i] = inverse[i];

        return true;
    }

    bool Matrix4::Invert(const Matrix4& m, Matrix4& outResult)
    {
        outResult = m;
        return outResult.Invert();
    }

    Matrix3 Matrix4::GetUpperLeft3x3() const
    {
        Matrix3 m;
        m[0] = Data[0];
        m[1] = Data[1];
        m[2] = Data[2];
        m[3] = Data[4];
        m[4] = Data[5];
        m[5] = Data[6];
        m[6] = Data[8];
        m[7] = Data[9];
        m[8] = Data[10];
        return m;
    }

    Vector3 Matrix4::GetTranslation() const
    {
        return Vector3(Data[3*4 + 0], Data[3*4 + 1], Data[3*4 + 2]);
    }

    Vector3 Matrix4::GetScaling() const
    {
        return Vector3(
            Sqrt((Data[0*4 + 0] * Data[0*4 + 0]) + (Data[0*4 + 1] * Data[0*4 + 1]) + (Data[0*4 + 2] * Data[0*4 + 2])),
            Sqrt((Data[1*4 + 0] * Data[1*4 + 0]) + (Data[1*4 + 1] * Data[1*4 + 1]) + (Data[1*4 + 2] * Data[1*4 + 2])),
            Sqrt((Data[2*4 + 0] * Data[2*4 + 0]) + (Data[2*4 + 1] * Data[2*4 + 1]) + (Data[2*4 + 2] * Data[2*4 + 2])));
    }

    const Matrix4& Matrix4::Identity()
    {
        static const float Data[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };

        static const Matrix4 id(Data);
        return id;
    }

    Matrix4 Matrix4::Translation(float x, float y, float z)
    {
        Matrix4 m = Identity();
        m.Data[12] = x;
        m.Data[13] = y;
        m.Data[14] = z;
        return m;
    }
    
    Matrix4 Matrix4::Translation(const Vector3& v)
    {
        return Translation(v.X, v.Y, v.Z);
    }

    Matrix4 Matrix4::RotationX(float angleRadians)
    {
        Matrix4 m;
        float* a = m.Data;
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
        float* a = m.Data;
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
        float* a = m.Data;
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
        float len = Length(axis);

        if (Abs(len) < FloatZeroTolerance) 
            return Identity();

        float lenInv = 1.0f / len;

        float x = axis.X * lenInv;
        float y = axis.Y * lenInv;
        float z = axis.Z * lenInv;

        Matrix4 m;
        float* a = m.Data;
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

    Matrix4 Matrix4::Rotation(const Quaternion& q)
    {
        return Rotation(q.Axis(), q.Angle());
    }

    Matrix4 Matrix4::Scaling(float x, float y, float z)
    {
        Matrix4 m = Identity();
        m.Data[0] = x;
        m.Data[5] = y;
        m.Data[10] = z;
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
}
