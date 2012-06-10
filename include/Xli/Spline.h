#ifndef __XLI_SPLINE_H__
#define __XLI_SPLINE_H__

#include <Xli/Math.h>

namespace Xli
{
	template <typename T> inline float Map(const T& a, const T& b, const T& t)
	{
		return float(t - a) / float(b - a);
	}

	template <typename T> inline T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a) * t;
	}

	template <typename T> inline T SCurve(const T& a, const T& b, float t)
	{
		return a + (b - a) * (0.5f - Cos(t * (float)PI) * 0.5f);
	}

	template <typename T> inline T Ramp(const T& a, const T& b, float t)
	{
		return a + (b - a) * (t*t);
	}

	template <typename T> inline T InterpolateCatmullRom(const T& a, const T& b, const T& c, const T& d, float t)
	{
		T p = b*3.0f - c*3.0f + d - a;
		T q = a*2.0f - b*5.0f + c*4.0f - d;
		T r = c - a;
		T s = b;
		return (p*(t*t*t) + q*(t*t) + r*t)*0.5f + s;
	}

	template <typename T> inline T InterpolateCubic(const T& a, const T& b, const T& c, const T& d, float t)
	{
		T p = (d - c) - (a - b);
		T q = (a - b) - p;
		T r = c - a;
		T s = b;
		return p*(t*t*t) + q*(t*t) + r*t + s;
	}


	template <typename Tx, typename Ty> class Spline: public Object
	{
	public:
		virtual Tx Start() = 0;
		virtual Tx End() = 0;

		virtual Ty Sample(const Tx& x) = 0;

		virtual Ty First() { return Sample(Start()); }
		virtual Ty Last() { return Sample(End()); }
	};

	template <typename Tx, typename Ty, typename TData = Ty> class KeyframedSpline: public Spline<Tx, Ty>
	{
	public:
		struct Key
		{
			Tx Time;
			TData Value;

			Key() {}
			Key(const Key& k): Time(k.Time), Value(k.Value) {}
			Key(const Tx& time, const Ty& value): Time(time), Value(value) {}

			bool operator < (const Key& k) const { return Time < k.Time; }
			bool operator > (const Key& k) const { return Time > k.Time; }
			bool operator == (const Key& k) const { return Time == k.Time; }
		};

	private:
		Array<Key> keys;
		int cache;

	public:
		KeyframedSpline(): cache(0)
		{
		}

		const Array<Key>& GetKeys() const
		{
			return keys;
		}

		virtual Tx Start() { return keys.First().Time; }
		virtual Tx End() { return keys.Last().Time; }

		int FindLeftIndex(const Tx& time)
		{
			if (keys.Length())
			{
				while (time >= keys[cache].Time)
				{
					if (cache == keys.Length() - 1) return cache;
					if (time < keys[cache + 1].Time) return cache;
					cache++;
				}
				while (time < keys[cache].Time)
				{
					if (cache == 0) return -1;
					cache--;
					if (time >= keys[cache].Time) return cache;
				}
			}

			return -1;
		}

		int FindNearestIndex(const Tx& time)
		{
			int l = FindLeftIndex(time);
			if (l < 0) return 0;
			int r = l + 1;
			if (r < keys.Length())
			{
				double ldif = time - GetTime(l);
				double rdif = GetTime(r) - time;
				if (rdif < ldif) return r;
			}
			return l;
		}

		int AddKey(const Tx& time, const Ty& value)
		{
			int i = FindLeftIndex(time) + 1;
			keys.Add(i, Key(time, value));
			return i;
		}

		void Trim()
		{
			keys.Trim();
		}

		void RemoveKey(int index)
		{
			if (cache == keys.Length() - 1 && cache > 0) cache--;
			keys.RemoveAt(index);
		}

		void ClearKeys()
		{
			keys.Clear();
			cache = 0;
		}

		int GetKeyCount() const
		{
			return keys.Length();
		}

		TData& GetValue(int index)
		{
			return keys[index].Value;
		}
		const TData& GetValue(int index) const
		{
			return keys[index].Value;
		}
		const Tx& GetTime(int index) const
		{
			return keys[index].Time;
		}
		void SetValue(int index, const TData& value)
		{
			keys[index].Value = value;
		}
		void SetTime(int index, const Tx& time)
		{
			if ((index > 0 && keys[index - 1].Time > time)
				|| (index < keys.Length() - 1 && keys[index + 1].Time < time))
			{
				Ty v = GetValue(index);
				RemoveKey(index);
				AddKey(time, v);
			}
			else
			{
				keys[index].Time = time;
			}
		}


		const TData& FindValue(const Tx& time)
		{
			return keys[Max(0, FindLeftIndex(time))].Value;
		}

		/**
			Finds two Keyframe-values and a interpolation factor.
			@param p Interpolation Factor is in [0..1] except in cases before start or after end.
		*/
		void FindValues(const Tx& time, TData& a, TData& b, float& p)
		{
			int i = FindLeftIndex(time);
			const Key& ka = keys[Max(i, 0)];
			const Key& kb = keys[Min(i + 1, keys.Length() - 1)];
			a = ka.Value;
			b = kb.Value;
			if (ka.Time == kb.Time) p = 0.0;
			else p = Map(ka.Time, kb.Time, time);
		}

		/**
			Finds four Keyframe-values and a interpolation factor.
			@param p Interpolation Factor is in [0..1] except in cases before start or after end.
		*/
		void FindValues(const Tx& time, TData& a, TData& b, TData& c, TData& d, float& p)
		{
			int i = FindLeftIndex(time);
			const Key& ka = keys[Max(i - 1, 0)];
			const Key& kb = keys[Max(i, 0)];
			const Key& kc = keys[Min(i + 1, keys.Length() - 1)];
			const Key& kd = keys[Min(i + 2, keys.Length() - 1)];
			a = ka.Value;
			b = kb.Value;
			c = kc.Value;
			d = kd.Value;
			if (kb.Time == kc.Time) p = 0.0;
			else p = Map(kb.Time, kc.Time, time);
		}
	};

	template <typename Tx, typename Ty> class StepSpline: public KeyframedSpline<Tx, Ty, Ty>
	{
	public:
		virtual Ty Sample(const Tx& time)
		{
			return this->FindValue(time);
		}
	};

	template <typename Tx, typename Ty> class LinearSpline: public KeyframedSpline<Tx, Ty, Ty>
	{
	public:
		virtual Ty Sample(const Tx& time)
		{
			Ty a, b;
			float p;
			this->FindValues(time, a, b, p);
			return Lerp(a, b, Saturate(p));
		}
	};


}

#endif
