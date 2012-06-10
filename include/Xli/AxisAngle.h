#ifndef __XLI_ANGLE_AXIS_H__
#define __XLI_ANGLE_AXIS_H__

#include <Xli/Vector3.h>
#include <Xli/Matrix4.h>

namespace Xli
{
	template <typename T> class AxisAnglet
	{
	public:
		union
		{
			struct
			{
				Vector3t<T> Axis;
				T Angle;
			};
			T Coord[4];
		};

		operator T*()
		{
			return Coord;
		}
		T& operator [] (int i)
		{
#ifdef XLI_RANGE_CHECK
			if (i >= 4 || i < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			return Coord[i];
		}
		String ToString() const
		{
			return Axis.ToString() + ", " + Angle;
		}

		AxisAnglet()
		{
		}
		AxisAnglet(const Vector3t<T>& axis, const T& angleRadians)
		{
			Axis = axis;
			Angle = angleRadians;
		}

		void SetAngleDegrees(const T& angleDegrees)
		{
			Angle = DegToRad(angleDegrees);
		}
		T AngleDegrees() const
		{
			return RadToDeg(Angle);
		}
	};

	typedef AxisAnglet<float> AxisAngle;
	typedef AxisAnglet<double> AxisAngled;
}

#endif
