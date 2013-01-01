#ifndef __XLI_RANDOM_H__
#define __XLI_RANDOM_H__

#include <Xli/Math.h>
#include <Xli/Vector2.h>
#include <Xli/Vector3.h>
#include <Xli/Vector4.h>

namespace Xli
{
	/**
		\ingroup Math

		A simple 32-bit pseudo-random number generator
	*/
	class Random
	{
		UInt32 z, w;

	public:
		Random(UInt32 seed = 1);
		void SetSeed(UInt32 seed);

		/**
			Returns a Random Integer in range [0..UInt32Max]
		*/
		UInt32 NextInt();

		/**
			Returns a Random Integer in range [0..high-1]
		*/
		UInt32 NextInt(UInt32 high);

		/**
			Returns a Random Integer in range [low..high-1]
		*/
		UInt32 NextInt(UInt32 low, UInt32 high);

		/**
			Returns a Random float in range [0..1]
		*/
		float NextFloat();

		/**
			Returns a Random float in range [-1..1]
		*/
		float NextFloatSign();

		/**
			Returns a Random float in range [min..max]
		*/
		float NextFloat(float min, float max);

		Vector2 NextVector2Sign();
		Vector3 NextVector3Sign();
		Vector4 NextVector4Sign();
	};

	/**
		\addtogroup Math

		Global Random Object
	*/
	extern Random Rand;
}

#endif

