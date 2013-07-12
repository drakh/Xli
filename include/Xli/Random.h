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
		UInt32 NextUInt32();

		/**
			Returns a Random Integer in range [0..high-1]
		*/
		int NextInt(int high);

		/**
			Returns a Random Integer in range [low..high-1]
		*/
		int NextInt(int low, int high);

		/**
			Returns a Random float in range [0..1]
		*/
		float NextFloat();

		/**
			Returns a Random float in range [min..max]
		*/
		float NextFloat(float min, float max);

		/**
			Returns a Random Vector2 in range [min..max]
		*/
		Vector2 NextVector2(float min, float max);

		/**
			Returns a Random Vector3 in range [min..max]
		*/
		Vector3 NextVector3(float min, float max);

		/**
			Returns a Random Vector4 in range [min..max]
		*/
		Vector4 NextVector4(float min, float max);
	};

	/**
		\addtogroup Math

		Global Random Object
	*/
	extern Random Rand;
}

#endif

