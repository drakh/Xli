#ifndef __XLI_MATRIX4_H__
#define __XLI_MATRIX4_H__

#include "Vector4.h"
#include "Matrix3.h"

namespace Xli
{
	/**
		\ingroup Math
	*/
	class Matrix4
	{
		float data[16];

	public:
		float* Data();
		const float* Data() const;
		operator float* ();
		operator const float* () const;
		
		float& operator [] (int i);

		String ToString() const;


		Matrix4();

		explicit Matrix4(const float* values);

		Matrix4(const Matrix4& m);

		Matrix4(float m11, float m12, float m13, float m14, 
			    float m21, float m22, float m23, float m24, 
			    float m31, float m32, float m33, float m34, 
			    float m41, float m42, float m43, float m44);

		Matrix4& operator = (const Matrix4& m);

		Matrix4 operator + (const Matrix4& m) const;
		Matrix4 operator - (const Matrix4& m) const;

		Matrix4 operator + (float s) const;
		Matrix4 operator - (float s) const;
		Matrix4 operator * (float s) const;

		Matrix4& operator += (const Matrix4& m);
		Matrix4& operator -= (const Matrix4& m);

		Matrix4& operator += (float s);
		Matrix4& operator -= (float s);
		Matrix4& operator *= (float s);

		Vector3 operator * (const Vector3& v) const;
		Vector4 operator * (const Vector4& v) const;
		Matrix4 operator * (const Matrix4& m) const;
		Matrix4& operator *= (const Matrix4& m);


		Matrix4 Transposed() const;

		void Transpose();
		bool Invert();

		Vector3 GetScale() const;

		Matrix4 Inverse() const;

		Matrix3 ToMatrix3() const;

		static const Matrix4& Identity();

		static Matrix4 Translation(float x, float y, float z);
		static Matrix4 Translation(const Vector3& v);

		static Matrix4 RotationX(float angleRadians);
		static Matrix4 RotationY(float angleRadians);
		static Matrix4 RotationZ(float angleRadians);

		static Matrix4 Rotation(const Vector3& axis, float angleRadians);
		static Matrix4 Rotation(float x, float y, float z, float angleRadians);

		static Matrix4 Scaling(float x, float y, float z);
		static Matrix4 Scaling(const Vector3& v);
		static Matrix4 Scaling(float s);

		static Matrix4 GLLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
		static Matrix4 GLPerspective(float fovRadians, float aspect, float zNear, float zFar);
		static Matrix4 GLOrtho(float left, float right, float bottom, float top, float nearval = (float)-1, float farval = (float)1);
	};
}

#endif
