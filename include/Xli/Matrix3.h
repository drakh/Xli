#ifndef __XLI_MATRIX3_H__
#define __XLI_MATRIX3_H__

#include "Math.h"
#include "Utils.h"
#include "Vector3.h"

namespace Xli
{
	/**
		\ingroup Math
	*/
	class Matrix3
	{
		float data[9];

	public:
		float* Data();
		const float* Data() const;

		operator float* ();
		operator const float* () const;

		float& operator [] (int i);

		String ToString() const;

		Matrix3();
		explicit Matrix3(const float* values);
		Matrix3(const Matrix3& m);
		Matrix3(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8);

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

		void Transpose();

		static const Matrix3& Identity();

		static Matrix3 RotationX(float angleRadians);
		static Matrix3 RotationY(float angleRadians);
		static Matrix3 RotationZ(float angleRadians);

		static Matrix3 Rotation(const Vector3& axis, float angleRadians);
		static Matrix3 Rotation(float x, float y, float z, float angleRadians);

		static Matrix3 Scaling(float x, float y, float z);
		static Matrix3 Scaling(const Vector3& v);
		static Matrix3 Scaling(float s);
	};
}

#endif
