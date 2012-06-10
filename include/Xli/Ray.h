#ifndef __XLI_RAY_H__
#define __XLI_RAY_H__

#include <Xli/Vector2.h>
#include <Xli/Vector3.h>
#include <Xli/Vector4.h>

namespace Xli
{
	template <typename VectorT> class Ray
	{
	public:
		VectorT Origin;
		VectorT Direction;

		Ray() {}
		Ray(const VectorT& origin, const VectorT& direction): Origin(origin), Direction(direction) {}
		Ray(const Ray& r): Origin(r.Origin), Direction(r.Direction) {}
	};

	typedef Ray<Vector2> Ray2;
	typedef Ray<Vector3> Ray3;
	typedef Ray<Vector4> Ray4;
}

#endif
