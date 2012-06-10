#ifndef __XLI_PLATFORM_TIMED_TRIGGER_H__
#define __XLI_PLATFORM_TIMED_TRIGGER_H__

#include <XliPlatform/Thread.h>
#include <XliPlatform/Mutex.h>
#include <XliPlatform/ConditionVariable.h>
#include <XliPlatform/Time.h>

namespace Xli
{
	class TimedTrigger: public Object
	{
	public:
		/**
			Fires the trigger.
			@return true if the trigger are done and should be removed, or false if trigger should not be removed and thus continue to be fired.
		*/
		virtual bool Fire(double deltaTime) = 0;
	};

	class TimedTriggers
	{
		class TriggerEntry
		{
		public:
			double StartTime;
			double Interval;
			Shared<TimedTrigger> Trigger;
		};

		class TimerJob: public ThreadJob
		{
		public:
			Mutex mutex;
			ConditionVariable cond;
			Array<TriggerEntry> triggers;
			virtual void Run();
		};

		Managed<TimerJob> job;
		Thread thread;

		TimedTriggers();

	public:
		static void Init();
		static void Shutdown();

		static void Add(double interval, TimedTrigger* trigger);
		static int GetCount();
	};
}

#endif
