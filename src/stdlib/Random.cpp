#include <Xli/Random.h>

namespace Xli
{
	Random::Random(UInt32 seed)
	{
		SetSeed(seed);
	}

	void Random::SetSeed(UInt32 seed)
	{
		if (!seed) seed = 1;
		z = seed;
		w = seed + 1;
	}

	UInt32 Random::NextInt()
	{
		z = 36969 * (z & UInt16Max) + (z >> 16);
		w = 18000 * (w & UInt16Max) + (w >> 16);
		return (z << 16) + w;
	}

	UInt32 Random::NextInt(UInt32 high)
	{
		return (UInt32)(NextFloat() * (float)high);
	}

	UInt32 Random::NextInt(UInt32 low, UInt32 high)
	{
		return (UInt32)(NextFloat() * (float)(high - low) + (float)low);
	}

	float Random::NextFloat()
	{
		return ((float)NextInt() / (float)UInt32Max);
	}

	float Random::NextFloatSign()
	{
		return NextFloat() * 2.0f - 1.0f;
	}

	float Random::NextFloat(float min, float max)
	{
		return NextFloat() * (max - min) + min;
	}
	
	Vector2 Random::NextVector2Sign()
	{
		return Vector2(NextFloatSign(), NextFloatSign());
	}

	Vector3 Random::NextVector3Sign()
	{
		return Vector3(NextFloatSign(), NextFloatSign(), NextFloatSign());
	}

	Vector4 Random::NextVector4Sign()
	{
		return Vector4(NextFloatSign(), NextFloatSign(), NextFloatSign(), NextFloatSign());
	}


	Random Rand;
}
