#include <Xli/TextWriter.h>
#include <Xli/DateTime.h>
#include <XliPlatform/Profiling.h>

namespace Xli
{
	double Profiling::Probe::AverageRunLength()
	{
		if (!Runs.Length()) return 0;
		double sum = 0.0;
		for (int i = 0; i < Runs.Length(); i++) sum += Runs[i].Duration;
		return sum / Runs.Length();
	}
	double Profiling::Probe::MinRunLength()
	{
		if (!Runs.Length()) return 0;
		double m = Runs[0].Duration;
		for (int i = 1; i < Runs.Length(); i++) if (Runs[i].Duration < m) m = Runs[i].Duration;
		return m;
	}
	double Profiling::Probe::MaxRunLength()
	{
		if (!Runs.Length()) return 0;
		double m = Runs[0].Duration;
		for (int i = 1; i < Runs.Length(); i++) if (Runs[i].Duration > m) m = Runs[i].Duration;
		return m;
	}
	Timestamp Profiling::Probe::FirstRun()
	{
		if (!Runs.Length()) return Timestamp();
		Timestamp m = Runs[0].When;
		for (int i = 1; i < Runs.Length(); i++) if (Runs[i].When < m) m = Runs[i].When;
		return m;
	}
	Timestamp Profiling::Probe::LastRun()
	{
		if (!Runs.Length()) return Timestamp();
		Timestamp m = Runs[0].When;
		for (int i = 1; i < Runs.Length(); i++) if (Runs[i].When > m) m = Runs[i].When;
		return m;
	}

	Array<Profiling::Probe*> Profiling::probes;
	
	int Profiling::NewProbe(const char* file, const char* func, int line, const char* label)
	{
		return probes.Add(new Profiling::Probe(file, func, line, label));
	}
	void Profiling::AddRun(int probeId, Timestamp when, double durationMs)
	{
		probes[probeId]->Runs.Add(Run(when, durationMs));
	}
	void Profiling::SaveHTMLLog(Stream* file, int maxRuns)
	{
		TextWriter w(file);
		w.WriteLine("<h1>Profiling report</h1>");

		for (int i = 0; i < probes.Length(); i++)
		{
			w.WriteLine("<h2>Probe #" + String(i) + ": '" + probes[i]->Label + "'</h2>");
			w.WriteLine("<p>In file '" + String(probes[i]->File) + "' line " + String(probes[i]->Line) + ", function '" + String(probes[i]->Func) + "'</p>");
			w.WriteLine("<h3>Stats</h3>");
			w.WriteLine("<table>");
			w.WriteLine("<tr><td>Run count:</td><td>" + String(probes[i]->Runs.Length()) + "</td></tr>");
			w.WriteLine("<tr><td>First run:</td><td>" + DateTime(probes[i]->FirstRun()).ToString() + "</td></tr>");
			w.WriteLine("<tr><td>Last run:</td><td>" + DateTime(probes[i]->LastRun()).ToString() + "</td></tr>");
			w.WriteLine("<tr><td><h4>Average time:</h4></td><td><h4>" + String(probes[i]->AverageRunLength()*1000.0) + " ms</h4></td></tr>");
			w.WriteLine("<tr><td>Min time:</td><td>" + String(probes[i]->MinRunLength()*1000.0) + " ms</td></tr>");
			w.WriteLine("<tr><td>Max time:</td><td>" + String(probes[i]->MaxRunLength()*1000.0) + " ms</td></tr>");
			w.WriteLine("</table>");
			w.WriteLine("<h3>Example runs</h3>");

			w.WriteLine("<table>");
			w.WriteLine("<tr><td>When</td><td>Duration</td></tr>");
			for (int r = 0; r < Min(maxRuns, probes[i]->Runs.Length()); r++)
			{
				w.WriteLine("<tr><td>" + DateTime(probes[i]->Runs[r].When).ToString() + "</td><td>" + String(probes[i]->Runs[r].Duration*1000.0) + " ms</td></tr>");
			}
			w.WriteLine("</table>");
		}
	}
}
