#ifndef __XLI_COLOR_H__
#define __XLI_COLOR_H__

#include <Xli/Saturated.h>
#include <Xli/String.h>

namespace Xli
{
	/**
		Template class for nonencoded RGBA colors.
		T is the type of each color component.
		@see Saturated
	*/
	template <typename T, typename TPixel, typename TPrincipal, int MaxValue> class Colort
	{
	public:
		union
		{
			struct { T R, G, B, A; };
			TPixel RGBA;
		};

		Colort() {}
		Colort(T v)
		{
			R = MaxValue;
			G = MaxValue;
			B = MaxValue;
			A = v;
		}
		Colort(T r, T g, T b, T a = MaxValue)
		{
			R = r;
			G = g;
			B = b;
			A = a;
		}
		Colort(const TPixel& p)
		{
			RGBA = p;
		}

		String ToString() const
		{
			return String("(") + String(R) + ", " + String(G) + ", " + String(B) + ", " + String(A) + ")";
		}

		Colort operator + (const Colort& c) { return Colort(R + c.R, G + c.G, B + c.B, A + c.A); }
		Colort operator - (const Colort& c) { return Colort(R - c.R, G - c.G, B - c.B, A - c.A); }
		Colort operator * (const Colort& c) { return Colort(R * c.R, G * c.G, B * c.B, A * c.A); }
		Colort operator + (const T& c) { return Colort(R + c, G + c, B + c, A + c); }
		Colort operator - (const T& c) { return Colort(R - c, G - c, B - c, A - c); }
		Colort operator * (const T& c) { return Colort(R * c, G * c, B * c, A * c); }

		T Luminance() const
		{
			T x = R;
			if (G > x) x = G;
			if (B > x) x = B;
			return x;
		}

		void RGBToYUV(float& y, float& u, float& v) const
		{
			y = (0.299f *(float)R)		+ (0.587f * (float)G)		+ (0.114f * (float)B);
			u = ((-0.14713f * (float)R) + (-0.28886f * (float)G)	+ (0.436f * (float)B)	+ 0.436f) / (0.436f * 2.0f);
			v = ((0.615f * (float)R)	+ (-0.51499f * (float)G)	- (0.10001f * (float)B) + 0.615f) / (0.615f * 2.0f);
		}

		static Colort FromYUV(float Y, float U, float V)
		{
			U = (U * (0.436f * 2.0f)) - 0.436f;
			V = (V * (0.615f * 2.0f)) - 0.615f;

			float fr = (Y + 1.13983f * V);
			float fg = (Y - 0.39465f * U - 0.58060f * V);
			float fb = (Y + 2.03211f * U);

			if (fr < 0) fr = 0;
			if (fg < 0) fg = 0;
			if (fb < 0) fb = 0;
			if (fr > 1.0f) fr = 1.0f;
			if (fg > 1.0f) fg = 1.0f;
			if (fb > 1.0f) fb = 1.0f;

			return Colort(fr, fg, fb, 1.0f);
		}

		void RGBToHSV(float& h, float& s, float& v) const
		{
			float min, max, delta;
			min = Min(Min(R,G),B);
			max = Max(Max(R,G),B);
			v = max;
			delta = max - min;
			if (delta != 0.0f)
				s = delta / max;
			else 
			{
				s = 0;
				h = 0;
				return;
			}
			if (R == max)
				h = (G - B) / delta;
			else if (G == max)
				h = 2.0f + (B - R) / delta;
			else
				h = 4.0f + (R - G) / delta;
			h /= 6.0f;
			if (h < 0.0f)
				h += 1.0f;

		}

		/***
			
		*/
		static Colort FromHSV(float h, float s, float v)
		{
			float r, g, b;
			int i;
			float f, p, q, t;

			if(s == 0) 
				return Colort(v,v,v,1.0f);
 
			h *= 6.0f;
			i = (int)Floor(h);
			f = h - i;
			p = v * (1 - s);
			q = v * (1 - s * f);
			t = v * (1 - s * (1 - f));
			switch(i) 
			{
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			default: r = v; g = p; b = q; break;
			} 
			return Colort(r, g, b, 1.0f);
		}

		Colort RGBEToRGB() const
		{
			Colort res;
			float f = powf(2.0f, A*255.0f-136.0f);
			res.R = R * 255.0f * f;
			res.G = G * 255.0f * f;
			res.B = B * 255.0f * f;
			res.A = 1.0f;
			return res;
		}

		Colort RGBToRGBE() const
		{
			float m = R;
			if (G > m) m = G;
			if (B > m) m = B;

			float e = ((logf(m) / logf(2)) + 136) / 255.0f;

			Colort<float,float,float,1000> res;
			res.R = R / m;
			res.G = G / m;
			res.B = B / m;
			res.A = e;
			return res;
		}

		
		float Hue() const
		{
			float max = R;
			if (G > max) max = G;
			if (B > max) max = B;
			float min = R;
			if (G < min) min = G;
			if (B < min) min = B;
			if (max == min) return 0;
			float d = (max-min);
			if (max == R) return fmod(60.0f * (R-B)/d + 360.0f, 360.0f);
			else if (max == G) return 60.0f * (B-R)/d + 120.0f;
			else return 60.0f * (R-G)/d + 240.0f;
		}

		static Colort Average4(Colort* c)
		{
			TPrincipal avgR = (c[0].R + c[1].R + c[2].R + c[3].R) / 4;
			TPrincipal avgG = (c[0].G + c[1].G + c[2].G + c[3].G) / 4;
			TPrincipal avgB = (c[0].B + c[1].B + c[2].B + c[3].B) / 4;
			TPrincipal avgA = (c[0].A + c[1].A + c[2].A + c[3].A) / 4;
			return Colort((T)avgR, (T)avgG, (T)avgB, (T)avgA);
		}

	};

	typedef Colort<UInt8, UInt32, int, 255> Coloru8;
	typedef Colort<float, float[4], float, 1> Color;
}

#endif
