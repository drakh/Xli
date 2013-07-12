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

	UInt32 Random::NextUInt32()
	{
		z = 36969 * (z & UInt16Max) + (z >> 16);
		w = 18000 * (w & UInt16Max) + (w >> 16);
		return (z << 16) + w;
	}

	int Random::NextInt(int high)
	{
		return (int)(NextFloat() * (float)high);
	}

	int Random::NextInt(int low, int high)
	{
		return (int)(NextFloat() * (float)(high - low) + (float)low);
	}

	float Random::NextFloat()
	{
		return ((float)NextUInt32() / (float)UInt32Max);
	}

	float Random::NextFloat(float min, float max)
	{
		return NextFloat() * (max - min) + min;
	}
	
	Vector2 Random::NextVector2(float min, float max)
	{
		return Vector2(NextFloat(min, max), NextFloat(min, max));
	}

	Vector3 Random::NextVector3(float min, float max)
	{
		return Vector3(NextFloat(min, max), NextFloat(min, max), NextFloat(min, max));
	}

	Vector4 Random::NextVector4(float min, float max)
	{
		return Vector4(NextFloat(min, max), NextFloat(min, max), NextFloat(min, max), NextFloat(min, max));
	}


	Random Rand;
}
