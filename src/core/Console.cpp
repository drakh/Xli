#include <Xli/Console.h>
#include <Xli/File.h>
#include <cstdlib>

namespace Xli
{
	static TextWriter* out = 0;
	static TextWriter* err = 0;
	static TextReader* in = 0;

	static int initCount = 0;

	void Console::Init()
	{
		if (!initCount)
		{
			out = new TextWriter(Manage(new File(stdout, false, true)));
			err = new TextWriter(Manage(new File(stderr, false, true)));
			in = new TextReader(Manage(new File(stdin, true, false)));
		}

		initCount++;
	}

	void Console::Done()
	{
		initCount--;

		if (!initCount)
		{
            out->GetStream()->Flush();
            err->GetStream()->Flush();
			delete out;
			delete err;
			delete in;
			out = 0;
			err = 0;
			in = 0;
		}
		else if (initCount < 0)
		{
			XLI_THROW_BAD_DELETE;
		}
	}

	static void AssertInit()
	{
		if (!initCount)
		{
			Console::Init();
			atexit(Console::Done);
		}
	}

	TextWriter* OutAccessor::operator ->()
	{
		AssertInit();
		return out;
	}

	OutAccessor::operator TextWriter*()
	{
		AssertInit();
		return out;
	}

	TextWriter* ErrAccessor::operator ->()
	{
		AssertInit();
		return err;
	}

	ErrAccessor::operator TextWriter*()
	{
		AssertInit();
		return err;
	}

	TextReader* InAccessor::operator ->()
	{
		AssertInit();
		return in;
	}

	InAccessor::operator TextReader*()
	{
		AssertInit();
		return in;
	}

	OutAccessor Out;
	ErrAccessor Err;
	InAccessor In;
}
