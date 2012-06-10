#include <XliPlatform/Pipe.h>
#include <Xli/TextReader.h>
#include <Xli/Console.h>

namespace Xli
{
	int Pipe::Exec(const CharString& cmd, Stream* out)
	{
		Managed<Pipe> p = Pipe::Create(cmd, FileModeRead);

		if (!out) out = Out->GetStream();
		
		char c;
		while (p->Read(&c, 1, 1) == 1) out->Write(&c, 1, 1);

		p->Close();
		return p->GetExitCode();
	}
}
