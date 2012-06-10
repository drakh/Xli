#ifndef __XLI_LETTERBOX_H__
#define __XLI_LETTERBOX_H__

#include "Vector2.h"
#include "Rectangle.h"

namespace Xli
{
	class Letterbox
	{
	public:
		static Vector2 GetScale(double monitorAspect, double contentAspect)
		{
			Vector2 ratio(1.0f, (float)(monitorAspect / contentAspect));

			if (ratio.Y > 1.0f)
			{
				ratio.X /= ratio.Y;
				ratio.Y = 1.0f;
			}

			return ratio;
		}

		static Vector2 GetScale(const Vector2i& windowSize, const Vector2i& contentSize)
		{
			return GetScale((double)windowSize.X / windowSize.Y, (double)contentSize.X / contentSize.Y);
		}

		static Vector2i GetSize(const Vector2i& windowSize, const Vector2& scale)
		{
			return (Vector2i)((Vector2)windowSize * scale);
		}
		
		static Vector2i GetSize(const Vector2i& windowSize, const Vector2i& contentSize)
		{
			return GetSize(windowSize, GetScale(windowSize, contentSize));
		}

		static Recti GetRect(const Vector2i& windowSize, const Vector2& scale)
		{
			Vector2i size = GetSize(windowSize, scale);

			Recti rect;
			rect.Left = (windowSize.X - size.X) / 2;
			rect.Top = (windowSize.Y - size.Y) / 2;
			rect.Right = rect.Left + size.X;
			rect.Bottom = rect.Top + size.Y;

			return rect;
		}
		
		static Recti GetRect(const Vector2i& windowSize, const Vector2i& contentSize)
		{
			return GetRect(windowSize, GetScale(windowSize, contentSize));
		}
	};
}


#endif
