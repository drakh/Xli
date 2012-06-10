#ifndef __XLI_DCT_H__
#define __XLI_DCT_H__

#include <Xli/Map2D.h>

namespace Xli
{
	template <typename T> class DCT
	{
	public:
		static void Transform2D(Map2D<T>& src, Map2D<T>& dst)
		{
			int w = src.GetWidth();
			int h = src.GetHeight();

			for (int u = 0; u < w; u++)
			{
				for (int v = 0; v < h; v++)
				{
					T sum = (T)0;
					for (int x = 0; x < w; x++)
					{
						for (int y = 0; y < h; y++)
						{
							sum += src.Get(x, y) * Cos(PI*(2*x+1)*u/(2*w)) * Cos(PI*(2*y+1)*v/(2*h));
						}
					}
					T c1 = sqrt(2.0/T(w));
					T c2 = sqrt(2.0/T(h));
					if (u == 0) c1 = 1.0 / sqrt(T(w));
					if (v == 0) c2 = 1.0 / sqrt(T(w));
					dst.Set(u, v, c1*c2*sum);
				}
			}
		}

		static void Inverse2D(Map2D<T>& src, Map2D<T>& dst)
		{
			int w = src.GetWidth();
			int h = src.GetHeight();

			for (int x = 0; x < w; x++)
			{
				for (int y = 0; y < h; y++)
				{
					T sum = (T)0;
					for (int u = 0; u < w; u++)
					{
						for (int v = 0; v < h; v++)
						{
							T c1 = sqrt(2.0/T(w));
							T c2 = sqrt(2.0/T(h));
							if (u == 0) c1 = 1.0 / sqrt(T(w));
							if (v == 0) c2 = 1.0 / sqrt(T(w));							
							sum += c1*c2 * src.Get(u, v) * Cos(PI*(2*x+1)*u/(2*w)) * Cos(PI*(2*y+1)*v/(2*h));
						}
					}
					dst.Set(x, y, sum);
				}
			}
		}
	};
}

#endif
