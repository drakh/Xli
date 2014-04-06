#ifndef __XLI_RANDOM_H__
#define __XLI_RANDOM_H__

#include <Xli/Vector2.h>
#include <Xli/Vector3.h>
#include <Xli/Vector4.h>

namespace Xli
{
    /**
        \ingroup Xli

        A simple 32-bit pseudo-random number generator
    */
    class Random
    {
        UInt32 z, w;

    public:
        Random(UInt32 seed = 1);
        
        void SetSeed(UInt32 seed);

        /**
            Returns a UInt32 in range [0..UInt32Max]
        */
        UInt32 NextUInt32();

        /**
            Returns an int in range [0..high-1]
        */
        int NextInt(int high);

        /**
            Returns an int in range [low..high-1]
        */
        int NextInt(int low, int high);

        /**
            Returns a float in range [0..1]
        */
        float NextFloat();

        /**
            Returns a float in range [min..max]
        */
        float NextFloat(float min, float max);

        /**
            Returns a Vector2 in range [min..max]
        */
        Vector2 NextVector2(float min, float max);

        /**
            Returns a random Vector3 in range [min..max]
        */
        Vector3 NextVector3(float min, float max);

        /**
            Returns a random Vector4 in range [min..max]
        */
        Vector4 NextVector4(float min, float max);
    };

    /**
        \addtogroup Xli
        @{
    */

    /**
        Global Random object
    */
    extern Random Rand;

    /** @} */
}

#endif

