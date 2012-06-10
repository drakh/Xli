#ifndef __XLI_GRAPHICS_GEOMETRY_SPLINE_H__
#define __XLI_GRAPHICS_GEOMETRY_SPLINE_H__

#include <Xli/Spline.h>
#include <Xli/Vector3.h>

namespace Xli
{
	class GeometrySpline: public Xli::Spline<float, Vector3>
	{
	public:
		enum Type
		{
			TypeUnknown = 0,
			TypeBezier = 1,
			TypeNURBS = 2,
		};

		virtual Type GetType() = 0;

		virtual float Start() { XLI_THROW_NOT_SUPPORTED(__FUNCTION__); }
		virtual float End() { XLI_THROW_NOT_SUPPORTED(__FUNCTION__); }
		virtual Vector3 Sample(const float& x) { XLI_THROW_NOT_SUPPORTED(__FUNCTION__); }
	};

	class BezierSpline: public GeometrySpline
	{
	public:
		virtual Type GetType() { return TypeBezier; }

		Array<Vector3> ControlPoints;
	};

	class NurbsSpline: public GeometrySpline
	{
	public:
		virtual Type GetType() { return TypeNURBS; }

		int Degree;
		Array<Vector3> ControlPoints;
		Array<float> Weights;
		Array<float> Knots;
	};
}

#endif
