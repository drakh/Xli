#ifndef __XLI_QUATERNION_H__
#define __XLI_QUATERNION_H__

#include <Xli/Vector4.h>

namespace Xli
{
    /**
        \ingroup XliCoreMath
    */
    class Quaternion
    {
    public:
        union
        {
            struct { float X, Y, Z, W; };
            float Coord[4];
        };

        operator float*();

        float& operator [] (int i);

        String ToString() const;

        Quaternion();
        Quaternion(float x, float y, float z, float w);
        Quaternion(const Vector4& v);
        Quaternion(const Quaternion& q);

        Quaternion& operator = (const Quaternion& v);

        Quaternion(const Vector3& axis, float angleRadians);

        Quaternion(float yaw, float pitch, float roll);

        static const Quaternion& Identity();
        
        Quaternion operator + (const Quaternion& q) const;
        Quaternion operator - (const Quaternion& q) const;

        Quaternion operator * (float s) const;
        Quaternion operator / (float s) const;

        Quaternion& operator += (const Quaternion& q);
        Quaternion& operator -= (const Quaternion& q);
        Quaternion& operator *= (const Quaternion& q);

        Quaternion& operator *= (float s);
        Quaternion& operator /= (float s);

        float Dot(const Quaternion& q) const;
        float LengthSquared() const;
        float Length() const;

        void Normalize();
        Quaternion Normalized() const;

        float AngleBetween(const Quaternion& q) const;

        float Angle() const;
        Vector3 Axis() const;

        Quaternion Inverse() const;
        Quaternion Slerp(const Quaternion& q, float t) const;
    };
}

#endif
