#ifndef __XLI_BOX_H__
#define __XLI_BOX_H__

#include <Xli/Vector2.h>
#include <Xli/Vector3.h>
#include <Xli/Exception.h>

namespace Xli
{
	/**
		Represents the coordinates of a 3D box.
		The rectangle is represented in a Left, Top, Right, Bottom, Front, Back-notation, where the X-axis increases
		in the right direction, where the Y-axis increases in the down direction, and the Z-axis increases in the back direction.
		The width of the box is defined to be (Right-Left), and the height is (Bottom-Top), depth is (Back-Front). Note that
		the elements at slice Right, Bottom and Back is not included in the rectangle.
	*/
	template <typename T> class Boxt
	{
	public:
		union
		{
			struct
			{
				// The lowest X-coordinate of the box
				T Left;

				// The lowest Y-coordinate of the box
				T Top;

				// The lowest Z-coordinate of the box
				T Front;

				// One plus the highest X-coordinate of the interior of the box
				T Right;

				// One plus the highest Y-coordinate of the interior of the box
				T Bottom;

				// One plus the highest Z-coordinate of the interior of the box
				T Back;
			};
			T data[6];
		};

		operator T*()
		{
			return data;
		}
		T& operator [] (int i)
		{
#ifdef XLI_RANGE_CHECK
			if (i >= 6 || i < 0)
			{
				XLI_THROW_INDEX_OUT_OF_BOUNDS;
			}
#endif
			return data[i];
		}
		String ToString() const
		{
			return String(Left) + ", " + Top + ", " + Front + ", " + Right + ", " + Bottom;
		}

		Boxt()
		{
		}
		Boxt(const T& left, const T& top, const T& front, const T& right, const T& bottom, const T& back)
		{
			Left = left;
			Top = top;
			Front = front;
			Right = right;
			Bottom = bottom;
			Back = back;
		}
		Boxt(const Boxt& v)
		{
			Left = v.Left;
			Top = v.Top;
			Front = v.Front;
			Right = v.Right;
			Bottom = v.Bottom;
			Back = v.Back;
		}
		template <typename U> explicit Boxt(const Boxt<U>& v)
		{
			Left = (T)v.Left;
			Top = (T)v.Top;
			Front = (T)v.Front;
			Right = (T)v.Right;
			Bottom = (T)v.Bottom;
			Back = (T)v.Back;
		}
		template <typename U> operator Boxt<U>() const
		{
			return Boxt<U>(Left, Top, Front, Right, Bottom, Back);
		}
		Boxt& operator = (const Boxt& v)
		{
			Left = v.Left;
			Top = v.Top;
			Front = v.Front;
			Right = v.Right;
			Bottom = v.Bottom;
			Back = v.Back;
			return *this;
		}

		Boxt(const Vector3t<T>& position, const Vector3t<T>& size)
		{
			Left = position.X;
			Top = position.Y;
			Front = position.Z;
			Right = Left + size.X;
			Bottom = Top + size.Y;
			Back = Front + size.Z;
		}

		/**
			Returns the i'th corner of the box.
			The corners are enumerated as follows: 0=upper left front, 1=lower left front, 2=lower right front, 3=upper right front, 4=upper left back, 5=lower left back, 6=lower right back, 7=upper right back
			If the index is out of range, an IndexOutOfBoundsException is thrown.
		*/
		Vector3t<T> GetCorner(int i)
		{
			switch (i)
			{
			case 0: return Vector3t<T>(Left, Top, Front);
			case 1: return Vector3t<T>(Left, Bottom, Front);
			case 2: return Vector3t<T>(Right, Bottom, Front);
			case 3: return Vector3t<T>(Right, Top, Front);
			case 4: return Vector3t<T>(Left, Top, Back);
			case 5: return Vector3t<T>(Left, Bottom, Back);
			case 6: return Vector3t<T>(Right, Bottom, Back);
			case 7: return Vector3t<T>(Right, Top, Back);
			}
			XLI_THROW_INDEX_OUT_OF_BOUNDS;
		}

		T Width() const
		{
			return Right - Left;
		}
		T Height() const
		{
			return Bottom - Top;
		}
		T Depth() const
		{
			return Back - Front;
		}

		Vector3t<T> Position() const
		{
			return Vector3t<T>(Left, Top, Front);
		}
		Vector3t<T> Size() const
		{
			return Vector2t<T>(Width(), Height(), Depth());
		}

		Vector3t<T> TopLeftFront() const
		{
			return Vector3t<T>(Left, Top, Front);
		}
		Vector3t<T> TopRightFront() const
		{
			return Vector3t<T>(Right, Top, Front);
		}
		Vector3t<T> BottomLeftFront() const
		{
			return Vector3t<T>(Left, Bottom, Front);
		}
		Vector3t<T> BottomRightFront() const
		{
			return Vector3t<T>(Right, Bottom, Front);
		}

		Vector3t<T> TopLeftBack() const
		{
			return Vector3t<T>(Left, Top, Back);
		}
		Vector3t<T> TopRightBack() const
		{
			return Vector3t<T>(Right, Top, Back);
		}
		Vector3t<T> BottomLeftBack() const
		{
			return Vector3t<T>(Left, Bottom, Back);
		}
		Vector3t<T> BottomRightBack() const
		{
			return Vector3t<T>(Right, Bottom, Back);
		}
		Vector3t<T> Center() const
		{
			return Vector3t<T>((Left+Right)/2, (Top+Bottom)/2, (Front+Back)/2);
		}

		T Volume() const
		{
			return Width() * Height() * Depth();
		}

		void Move(const T& dX, const T& dY, const T& dZ)
		{
			Left += dX;
			Top += dY;
			Front += dZ;
			Right += dX;
			Bottom += dY;
			Back += dZ;
		}
		void MoveTo(const T& x, const T& y, const T& z)
		{
			Move(x-Left, y-Top, z-Front);
		}
		void Resize(const T& width, const T& height, const T& depth)
		{
			Right = Left + width;
			Bottom = Top + height;
			Back = Front + depth;
		}

		bool Intersects(const T& x, const T& y, const T& z) const
		{
			if (x < Left) return false;
			if (x >= Right) return false;
			if (y < Top) return false;
			if (y >= Bottom) return false;
			if (z < Front) return false;
			if (z >= Back) return false;
			return true;
		}

		/**
			Clip this rectangle against a scissor rectangle.
		*/
		Boxt Clip(const Boxt& scissor)
		{
			Boxt r;
			r.Left = Max(Left, scissor.Left);
			r.Right = Min(Right, scissor.Right);
			r.Top = Max(Top, scissor.Top);
			r.Bottom = Min(Bottom, scissor.Bottom);
			r.Front = Max(Front, scissor.Front);
			r.Back = Max(Back, scissor.Back);
			return r;

		}
	};

	typedef Boxt<float> Box;
	typedef Boxt<double> Boxd;
	typedef Boxt<int> Boxi;
	typedef Boxt<unsigned int> Boxu;

	typedef Boxt<Int64> Boxi64;
	typedef Boxt<Int32> Boxi32;
	typedef Boxt<Int16> Boxi16;
	typedef Boxt<Int8> Boxi8;
	typedef Boxt<UInt64> Boxu64;
	typedef Boxt<UInt32> Boxu32;
	typedef Boxt<UInt16> Boxu16;
	typedef Boxt<UInt8> Boxu8;
}


#endif
