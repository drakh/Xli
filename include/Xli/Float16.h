#ifndef __XLI_MEDIA_FLOAT16_H__
#define __XLI_MEDIA_FLOAT16_H__

#include <Xli/BaseTypes.h>

namespace Xli
{
	class Float16
	{
		UInt16 d;

		static inline UInt16 floatToHalf(float i)
		{
			union { float f; UInt32 i; } v;
			v.f = i;
			return floatToHalfI(v.i);
		}

		static UInt16 floatToHalfI(UInt32 i)
		{
			register int s =  (i >> 16) & 0x00008000;
			register int e = ((i >> 23) & 0x000000ff) - (127 - 15);
			register int m =   i        & 0x007fffff;

			if (e <= 0)
			{
				if (e < -10)return 0;
				m = (m | 0x00800000) >> (1 - e);
				return s | (m >> 13);
			}
			else if (e == 0xff - (127 - 15))
			{
				if (m == 0) return s | 0x7c00; // Inf
				else    
				{
					m >>= 13;
					return s | 0x7c00 | m | (m == 0); // NAN
				}
			}
			else
			{
				if (e > 30) return s | 0x7c00; // Overflow
				return s | (e << 10) | (m >> 13);
			}
		}

		static inline float halfToFloat(UInt16 y)
		{
			union { float f; UInt32 i; } v;
			v.i = halfToFloatI(y);
			return v.f;
		}
		static UInt32 halfToFloatI(UInt16 y)
		{
			register int s = (y >> 15) & 0x00000001;
			register int e = (y >> 10) & 0x0000001f;
			register int m =  y        & 0x000003ff;

			if (e == 0)
			{
				if (m == 0) return s << 31; // Plus or minus zero
				else 
				{
					// Denormalized ...
					while (!(m & 0x00000400))
					{
						m <<= 1;
						e -=  1;
					}
					e += 1;
					m &= ~0x00000400;
				}
			}
			else if (e == 31)
			{
				if (m == 0) return (s << 31) | 0x7f800000; // Inf
				else return (s << 31) | 0x7f800000 | (m << 13); // NaN
			}

			e = e + (127 - 15);
			m = m << 13;

			return (s << 31) | (e << 23) | m;
		}

	public:
		Float16()
		{
		}
		Float16(float f)
		{
			d = floatToHalf(f);
		}
		Float16(const Float16& f)
		{
			d = f.d;
		}
		operator float() 
		{ 
			return halfToFloat(d); 
		}

		Float16 operator + (const Float16& h) const { return halfToFloat(d) + halfToFloat(h.d); }
		Float16 operator - (const Float16& h) const { return halfToFloat(d) - halfToFloat(h.d); }
		Float16 operator * (const Float16& h) const { return halfToFloat(d) * halfToFloat(h.d); }
		Float16 operator / (const Float16& h) const { return halfToFloat(d) / halfToFloat(h.d); }
        
		Float16 operator += (const Float16& h) { *this = *this + h; return *this; }
		Float16 operator -= (const Float16& h) { *this = *this - h; return *this; }
		Float16 operator *= (const Float16& h) { *this = *this * h; return *this; }
		Float16 operator /= (const Float16& h) { *this = *this / h; return *this; }
	};
}


#endif
