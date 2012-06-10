#ifndef __XLI_LINE_SEGMENT_H__
#define __XLI_LINE_SEGMENT_H__

#include <Xli/Vector2.h>
#include <Xli/Vector3.h>
#include <Xli/Vector4.h>

namespace Xli
{
	template <typename VectorT> class LineSegment
	{
	public:
		VectorT A, B;

		LineSegment() {}
		LineSegment(const VectorT& a, const VectorT& b): A(a), B(b)	{}
		LineSegment(const LineSegment& s)
		{
			this->A = s.A;
			this->B = s.B;
		}
	};

	template <typename T> class LineSegment2t: public LineSegment<Vector2t<T> >
	{
	public:
		LineSegment2t() {}
		LineSegment2t(const Vector2t<T>& a, const Vector2t<T>& b): LineSegment<Vector2t<T> >(a, b)	{}
		LineSegment2t(const LineSegment2t& s): LineSegment<Vector2t<T> >(s) {}

		bool Intersects(const LineSegment2t& s, Vector2t<T>& point)
		{
			T Ax = this->A.X;
			T Ay = this->A.Y;
			T Bx = this->B.X;
			T By = this->B.Y;
			T Cx = s.A.X;
			T Cy = s.A.Y;
			T Dx = s.B.X;
			T Dy = s.B.Y;

			if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) return false;

			if ((Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy) || (Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy)) return false;

			Bx -= Ax; By -= Ay;
			Cx -= Ax; Cy -= Ay;
			Dx -= Ax; Dy -= Ay;

			T distAB = Sqrt(Bx*Bx + By*By);

			T theCos = Bx/distAB;
			T theSin = By/distAB;
			T newX = Cx*theCos+Cy*theSin;
			Cy = Cy*theCos-Cx*theSin;
			Cx = newX;
			newX = Dx*theCos+Dy*theSin;
			Dy = Dy*theCos-Dx*theSin;
			Dx = newX;

			if ((Cy<(T)0.0 && Dy<(T)0.0) || (Cy>=(T)0.0 && Dy>=(T)0.0)) return false;

			T ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

			if (ABpos<(T)0.0 || ABpos>distAB) return false;

			point.X = Ax+ABpos*theCos;
			point.Y = Ay+ABpos*theSin;

			return true;
		}

		void DistanceTo(const Vector2t<T>& point, float& distanceSegment, float& distanceLine)
		{
			T cx = point.X;
			T cy = point.Y;
			T ax = this->A.X;
			T ay = this->A.Y;
			T bx = this->B.X;
			T by = this->B.Y;
			T r_numerator = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
			T r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
			T r = r_numerator / r_denomenator;
			T px = ax + r*(bx-ax);
			T py = ay + r*(by-ay);
			T s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;
			distanceLine = Abs(s)*Sqrt(r_denomenator);
			T xx = px;
			T yy = py;

			if ( (r >= (T)0) && (r <= (T)1) )
			{
					distanceSegment = distanceLine;
			}
			else
			{

					T dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
					T dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
					if (dist1 < dist2)
					{
							xx = ax;
							yy = ay;
							distanceSegment = Sqrt(dist1);
					}
					else
					{
							xx = bx;
							yy = by;
							distanceSegment = Sqrt(dist2);
					}


			}
		}
	};

	typedef LineSegment2t<float> LineSegment2;
	typedef LineSegment2t<double> LineSegment2d;

	typedef LineSegment<Vector3> LineSegment3;
	typedef LineSegment<Vector4> LineSegment4;
}


#endif
