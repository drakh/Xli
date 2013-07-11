#include <Xli/Quaternion.h>

namespace Xli
{
	Quaternion::operator float*()
	{
		return Coord;
	}
	
	float& Quaternion::operator [] (int i)
	{
#ifdef XLI_RANGE_CHECK
		if (i >= 4 || i < 0)
		{
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
		}
#endif
		return Coord[i];
	}
	
	String Quaternion::ToString() const
	{
		return String(X) + ", " + Y + ", " + Z + ", " + W;
	}

	Quaternion::Quaternion()
	{
	}

	Quaternion::Quaternion(float x, float y, float z, float w)
	{
		X = x;
		Y = y;
		Z = z;
		W = w;
	}

	Quaternion::Quaternion(const Vector4& v)
	{
		X = v.X;
		Y = v.Y;
		Z = v.Z;
		W = v.W;
	}

	Quaternion::Quaternion(const Quaternion& q)
	{
		X = q.X;
		Y = q.Y;
		Z = q.Z;
		W = q.W;
	}

	Quaternion& Quaternion::operator = (const Quaternion& v)
	{
		X = v.X;
		Y = v.Y;
		Z = v.Z;
		W = v.W;
		return *this;
	}

	Quaternion::Quaternion(const Vector3& axis, float angleRadians)
	{
		float d = Xli::Length(axis);
		if (d == float(0.0)) return;
		float s = Sin(angleRadians * float(0.5)) / d;

		X = axis.X * s;
		Y = axis.Y * s;
		Z = axis.Z * s;
		W = Cos(angleRadians * float(0.5));
	}

	Quaternion::Quaternion(float yaw, float pitch, float roll)
	{
		float halfYaw = yaw * float(0.5);
		float halfPitch = pitch * float(0.5);
		float halfRoll = roll * float(0.5);

		float cosYaw = Cos(halfYaw);
		float sinYaw = Sin(halfYaw);
		float cosPitch = Cos(halfPitch);
		float sinPitch = Sin(halfPitch);
		float cosRoll = Cos(halfRoll);
		float sinRoll = Sin(halfRoll);

		X = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		Y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		Z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		W = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
	}
    
	const Quaternion& Quaternion::Identity()
	{
		static const Quaternion id(0,0,0,1);
		return id;
	}

	Quaternion Quaternion::operator + (const Quaternion& q) const
	{
		Quaternion r;
		r.X = X + q.X;
		r.Y = Y + q.Y;
		r.Z = Z + q.Z;
		r.W = W + q.W;
		return r;
	}

	Quaternion Quaternion::operator - (const Quaternion& q) const
	{
		Quaternion r;
		r.X = X - q.X;
		r.Y = Y - q.Y;
		r.Z = Z - q.Z;
		r.W = W - q.W;
		return r;
	}

	Quaternion Quaternion::operator * (float s) const
	{
		Quaternion r;
		r.X = X * s;
		r.Y = Y * s;
		r.Z = Z * s;
		r.W = W * s;
		return r;
	}

	Quaternion Quaternion::operator / (float s) const
	{
		return *this * (1.0f / s);
	}

	Quaternion& Quaternion::operator += (const Quaternion& q)
	{
		X += q.X;
		Y += q.Y;
		Z += q.Z;
		W += q.W;
		return *this;
	}

	Quaternion& Quaternion::operator -= (const Quaternion& q)
	{
		X -= q.X;
		Y -= q.Y;
		Z -= q.Z;
		W -= q.W;
		return *this;
	}

	Quaternion& Quaternion::operator *= (const Quaternion& q)
	{
		X = W * q.X + X * q.W + Y * q.Z - Z * q.Y;
		Y = W * q.Y + Y * q.W + Z * q.X - X * q.Z;
		Z = W * q.Z + Z * q.W + X * q.Y - Y * q.X;
		W = W * q.W - X * q.X - Y * q.Y - Z * q.Z;
		return *this;
	}

	Quaternion& Quaternion::operator *= (float s)
	{
		X *= s;
		Y *= s;
		Z *= s;
		W *= s;
		return *this;
	}

	Quaternion& Quaternion::operator /= (float s)
	{
		return *this *= (1.0f / s);
	}

	float Quaternion::Dot(const Quaternion& q) const
	{
		return X * q.X + Y * q.Y + Z * q.Z + W * q.W;
	}

	float Quaternion::LengthSquared() const
	{
		return Dot(*this);
	}

	float Quaternion::Length() const
	{
		return Sqrt(LengthSquared());
	}

	void Quaternion::Normalize()
	{
		*this /= Length();
	}

	Quaternion Quaternion::Normalized() const
	{
		return *this / Length();
	}

	float Quaternion::AngleBetween(const Quaternion& q) const
	{
		float s = Sqrt(LengthSquared() * q.LengthSquared());
		if (s == float(0.0)) return float(0.0);
		return ArcCos(Dot(q)/s);
	}

	float Quaternion::Angle() const
	{
		return float(2.0) * ArcCos(W);
	}

	Vector3 Quaternion::Axis() const
	{
		float s2 = 1.0f - Pow(W, 2.0f);
		if (s2 < 0.00001f) return Vector3(1.0f, 0.0f, 0.0f);
		float s = Sqrt(s2);
		return Vector3(X / s, Y / s, Z / s);
	}

	Quaternion Quaternion::Inverse() const
	{
		return Quaternion(-X, -Y, -Z, W);
	}

	Quaternion Quaternion::Slerp(const Quaternion& q, float t) const
	{
		if (t <= 0.0f) return *this;
		if (t >= 1.0f) return q;

		float theta = q.Angle();

		if (theta != 0.0f)
		{
			float d = 1.0f / Sin(theta);
			float s0 = Sin((1.0f - t) * theta);
			float s1 = Sin(t * theta);

			Quaternion r;
			r.X = (X * s0 + q.X * s1) * d;
			r.Y = (Y * s0 + q.Y * s1) * d;
			r.Z = (Z * s0 + q.Z * s1) * d;
			r.W = (W * s0 + q.W * s1) * d;
			return r;
		}

		return *this;
	}
}
