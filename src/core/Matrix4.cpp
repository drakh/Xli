#include <Xli/Matrix4.h>

namespace Xli
{
    float* Matrix4::DataPtr()
    {
        return data;
    }

    const float* Matrix4::DataPtr() const
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
        
        for (int i = 1; i < 16; i++) 
            s = s + ", " + data[i];

        return s;
    }

    Matrix4::Matrix4()
    {
    }

    Matrix4::Matrix4(const float* values)
    {
        for (int i = 0; i < 16; i++) 
            data[i] = values[i];
    }

    Matrix4::Matrix4(const Matrix4& m)
    {
        for (int i = 0; i < 16; i++) 
            data[i] = m.data[i];
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
        for (int i = 0; i < 16; i++) 
            data[i] = m.data[i];

        return *this;
    }

    Matrix4 Matrix4::operator + (const Matrix4& m) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++)
            r.data[i] = data[i] + m.data[i];

        return r;
    }

    Matrix4 Matrix4::operator - (const Matrix4& m) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++) 
            r.data[i] = data[i] - m.data[i];

        return r;
    }

    Matrix4 Matrix4::operator + (float s) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++) 
            r.data[i] = data[i] + s;

        return r;
    }

    Matrix4 Matrix4::operator - (float s) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++) 
            r.data[i] = data[i] - s;

        return r;
    }

    Matrix4 Matrix4::operator * (float s) const
    {
        Matrix4 r;

        for (int i = 0; i < 16; i++) 
            r.data[i] = data[i] * s;

        return r;
    }

    Matrix4& Matrix4::operator += (const Matrix4& m)
    {
        for (int i = 0; i < 16; i++) 
            data[i] += m.data[i];

        return *this;
    }

    Matrix4& Matrix4::operator -= (const Matrix4& m)
    {
        for (int i = 0; i < 16; i++) 
            data[i] -= m.data[i];

        return *this;
    }

    Matrix4& Matrix4::operator += (float s)
    {
        for (int i = 0; i < 16; i++) 
            data[i] += s;

        return *this;
    }

    Matrix4& Matrix4::operator -= (float s)
    {
        for (int i = 0; i < 16; i++) 
            data[i] -= s;

        return *this;
    }
    
    Matrix4& Matrix4::operator *= (float s)
    {
        for (int i = 0; i < 16; i++) 
            data[i] *= s;

        return *this;
    }

    Vector3 Matrix4::operator * (const Vector3& v) const
    {
        Vector3 r;
        r.X = data[0] * v.X + data[4] * v.Y + data[8] * v.Z + data[12];
        r.Y = data[1] * v.X + data[5] * v.Y + data[9] * v.Z + data[13];
        r.Z = data[2] * v.X + data[6] * v.Y + data[10] * v.Z + data[14];
        return r;
    }

    Vector4 Matrix4::operator * (const Vector4& v) const
    {
        Vector4 r;
        r.X = data[0] * v.X + data[4] * v.Y + data[8] * v.Z + data[12] * v.W;
        r.Y = data[1] * v.X + data[5] * v.Y + data[9] * v.Z + data[13] * v.W;
        r.Z = data[2] * v.X + data[6] * v.Y + data[10] * v.Z + data[14] * v.W;
        r.W = data[3] * v.X + data[7] * v.Y + data[11] * v.Z + data[15] * v.W;
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
                res.data[idx+j] =
                    m.data[idx + 0] * data[0 * 4 + j] +
                    m.data[idx + 1] * data[1 * 4 + j] +
                    m.data[idx + 2] * data[2 * 4 + j] +
                    m.data[idx + 3] * data[3 * 4 + j];
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
        t.data[0] = m.data[0];
        t.data[1] = m.data[4];
        t.data[2] = m.data[8];
        t.data[3] = m.data[12];
        t.data[4] = m.data[1];
        t.data[5] = m.data[5];
        t.data[6] = m.data[9];
        t.data[7] = m.data[13];
        t.data[8] = m.data[2];
        t.data[9] = m.data[6];
        t.data[10] = m.data[10];
        t.data[11] = m.data[14];
        t.data[12] = m.data[3];
        t.data[13] = m.data[7];
        t.data[14] = m.data[11];
        t.data[15] = m.data[15];
        return t;
    }

    void Matrix4::Transpose()
    {
        *this = Transpose(*this);
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

    Vector3 Matrix4::GetTranslation() const
    {
        return Vector3(data[3*4 + 0], data[3*4 + 1], data[3*4 + 2]);
    }

    Vector3 Matrix4::GetScaling() const
    {
        return Vector3(
            Sqrt((data[0*4 + 0] * data[0*4 + 0]) + (data[0*4 + 1] * data[0*4 + 1]) + (data[0*4 + 2] * data[0*4 + 2])),
            Sqrt((data[1*4 + 0] * data[1*4 + 0]) + (data[1*4 + 1] * data[1*4 + 1]) + (data[1*4 + 2] * data[1*4 + 2])),
            Sqrt((data[2*4 + 0] * data[2*4 + 0]) + (data[2*4 + 1] * data[2*4 + 1]) + (data[2*4 + 2] * data[2*4 + 2])));
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
        float len = Length(axis);

        if (Abs(len) < FloatZeroTolerance) 
            return Identity();

        float lenInv = 1.0f / len;

        float x = axis.X * lenInv;
        float y = axis.Y * lenInv;
        float z = axis.Z * lenInv;

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

    Matrix4 Matrix4::Rotation(const Quaternion& q)
    {
        return Rotation(q.Axis(), q.Angle());
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
}
