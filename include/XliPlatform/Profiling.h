#ifndef __XLI_PROFILING_H__
#define __XLI_PROFILING_H__

#include <Xli/Array.h>
#include <Xli/Stream.h>
#include <Xli/BaseTypes.h>
#include <XliPlatform/Time.h>

namespace Xli
{
	class Profiling
	{
		struct Run
		{
			Timestamp When;
			double Duration;
			Run(Timestamp when, double duration): When(when), Duration(duration) {}
			Run() {}
		};
		struct Probe
		{
			const char *File, *Func, *Label;
			int Line;
			Array<Run> Runs;
			double AverageRunLength();
			double MinRunLength();
			double MaxRunLength();
			Timestamp FirstRun();
			Timestamp LastRun();
			Probe(const char* file, const char* func, int line, const char* label): File(file), Func(func), Line(line), Label(label) {}
		};
		static Array<Probe*> probes;
	public:
		static int NewProbe(const char* file, const char* func, int line, const char* label);
		static void AddRun(int probeId, Timestamp when, double durationMs);
		static void SaveHTMLLog(Stream* file, int maxRuns=5);
	};
}

#define XLI_PROFILE_START(label) \
	{\
		static int prof_probe_id = Profiling::NewProbe(__FILE__, __FUNCTION__, __LINE__, label);\
		Timestamp prof_when = Xli::GetTimestamp();\
		double prof_t = Xli::GetTime();\
		{

#define XLI_PROFILE_END() \
		prof_t = Xli::GetTime()-prof_t;\
		Profiling::AddRun(prof_probe_id, prof_when, prof_t);\
		}\
	}

#define XLI_PROFILE(x) XLI_PROFILE_START(#x); x; XLI_PROFILE_END();




#endif
