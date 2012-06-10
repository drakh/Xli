#ifndef __XLI_PLATFORM_PIPE_H__
#define __XLI_PLATFORM_PIPE_H__

#include <Xli/File.h>

namespace Xli
{
	class Pipe: public Stream
	{
	public:
		virtual int GetExitCode() const = 0;

		static Pipe* Create(const CharString& cmd, FileMode mode);
		
		/*
		 * Executes a process and returns when it is finished.
		 * @param cmd Command to execute
		 * @param out Stream to recieve output. If NULL, output is written to stdout
		 * @return The exit code of the executed process
		 */
		static int Exec(const CharString& cmd, Stream* out = 0);
	};
}

#endif
