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

		Matrix4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15);

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

		static Matrix4 GLLookAt(Vector3t<float> eye, Vector3t<float> center, Vector3t<float> up);
		static Matrix4 GLPerspective(float fovRadians, float aspect, float zNear, float zFar);
		static Matrix4 GLOrtho(float left, float right, float bottom, float top, float nearval = (float)-1, float farval = (float)1);
	};
}

#endif
