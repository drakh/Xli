#include <XliPlatform/TimedTrigger.h>

namespace Xli
{
	void TimedTriggers::TimerJob::Run()
	{
		while (!IsStopped())
		{
			mutex.Lock();

			if (!triggers.Length())
			{
				cond.Wait(mutex);
			}

			double time = GetTime();

			for (int i = 0; i < triggers.Length(); i++)
			{
				double dt = time - triggers[i].StartTime;

				if (dt >= triggers[i].Interval)
				{
					if (triggers[i].Trigger->Fire(dt))
					{
						triggers[i].Trigger = 0;
						triggers.SwapRemoveAt(i);
						i--;
					}
					else
					{
						triggers[i].StartTime = time;
					}
				}
			}

			mutex.Unlock();
			Sleep(1);
		}
	}

	TimedTriggers::TimedTriggers()
	{
		job = new TimerJob();
		thread.Start(job);
	}

	static TimedTriggers* triggers = 0;
	static int TriggerRefCount = 0;

	void TimedTriggers::Init()
	{
		if (!TriggerRefCount)
		{
			triggers = new TimedTriggers();
		}

		TriggerRefCount++;
	}

	void TimedTriggers::Shutdown()
	{
		TriggerRefCount--;

		if (TriggerRefCount == 0)
		{
			delete triggers;
		}
		else if (TriggerRefCount < 0)
		{
			XLI_THROW_BAD_DELETE;
		}
	}

	static void AssertInit()
	{
		if (!TriggerRefCount)
		{
			TimedTriggers::Init();
			atexit(TimedTriggers::Shutdown);
		}
	}

	void TimedTriggers::Add(double interval, TimedTrigger* trigger)
	{
		TriggerEntry tt;
		tt.StartTime = GetTime();
		tt.Interval = interval;
		tt.Trigger = trigger;

		AssertInit();
		triggers->job->mutex.Lock();
		triggers->job->triggers.Add(tt);
		triggers->job->cond.Signal();
		triggers->job->mutex.Unlock();
	}

	int TimedTriggers::GetCount()
	{
		AssertInit();
		triggers->job->mutex.Lock();
		int c = triggers->job->triggers.Length();
		triggers->job->mutex.Unlock();
		return c;
	}
}
