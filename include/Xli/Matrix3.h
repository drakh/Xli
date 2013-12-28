#ifndef __XLI_MATRIX3_H__
#define __XLI_MATRIX3_H__

#include "Math.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace Xli
{
    /**
        \ingroup XliCoreMath
    */
    class Matrix3
    {
        float data[9];

    public:
        Matrix3();
        Matrix3(const Matrix3& m);
        explicit Matrix3(const float* values);

        Matrix3(float m11, float m12, float m13, 
                float m21, float m22, float m23, 
                float m31, float m32, float m33);

        static const Matrix3& Identity();

        static Matrix3 RotationX(float angleRadians);
        static Matrix3 RotationY(float angleRadians);
        static Matrix3 RotationZ(float angleRadians);

        static Matrix3 Rotation(const Quaternion& q);
        static Matrix3 Rotation(const Vector3& axis, float angleRadians);
        static Matrix3 Rotation(float x, float y, float z, float angleRadians);

        static Matrix3 Scaling(float x, float y, float z);
        static Matrix3 Scaling(const Vector3& v);
        static Matrix3 Scaling(float s);

        float* Data();
        const float* Data() const;

        operator float* ();
        operator const float* () const;

        float& operator [] (int i);

        String ToString() const;
        Quaternion ToQuaternion() const;

        void Transpose();

        Matrix3& operator = (const Matrix3& m);

        Matrix3 operator + (const Matrix3& m) const;
        Matrix3 operator - (const Matrix3& m) const;

        Matrix3 operator + (float s) const;
        Matrix3 operator - (float s) const;
        Matrix3 operator * (float s) const;

        Matrix3& operator += (const Matrix3& m);
        Matrix3& operator -= (const Matrix3& m);

        Matrix3& operator += (float s);
        Matrix3& operator -= (float s);
        Matrix3& operator *= (float s);

        Vector3 operator * (const Vector3& v) const;

        Matrix3 operator * (const Matrix3& m) const;
        Matrix3& operator *= (const Matrix3& m);
    };
}

#endif
