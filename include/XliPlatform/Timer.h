#ifndef __XLI_PLATFORM_TIMER_H__
#define __XLI_PLATFORM_TIMER_H__

#include <XliPlatform/Time.h>

namespace Xli
{
	class Timer
	{
		double startTime;
		double lastTime;
		double currentTime;

	public:
		Timer()
		{
			Reset();
		}
		void Reset()
		{
			startTime = Xli::GetTime();
			lastTime = startTime;
			currentTime = startTime;
		}
		void Update()
		{
			lastTime = currentTime;
			currentTime = Xli::GetTime();
		}
		double GetTime() const
		{
			return currentTime - startTime;
		}
		double GetInterval() const
		{
			return currentTime - lastTime;
		}
	};

	class FpsCounter
	{
	    double t, fps, freq;
	    int frames;

    public:
	    FpsCounter(double freq = 1.0)
	    {
	        this->freq = freq;
	        t = 0.0;
	        frames = 0;
	        fps = -1.0;
	    }

	    bool Update(double dt)
	    {
	        frames++;
	        t += dt;

	        if (t >= freq)
	        {
                fps = (double)frames / t;
                frames = 0;
                t = 0.0;
                return true;
	        }

	        return false;
	    }

	    double GetFps() const
	    {
	        return fps;
	    }
	};
}

#endif
