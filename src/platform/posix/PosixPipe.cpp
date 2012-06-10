#include <XliPlatform/Pipe.h>

namespace Xli
{
	class PosixPipe: public Pipe
	{
		FILE* fp;
		int stat;
		
	public:
		PosixPipe(const CharString& cmd, FileMode mode)
		{
			const char* m = 0;
			if (mode == FileModeRead) m = "r";
			else if (mode == FileModeWrite) m = "w";
			else XLI_THROW("Invalid FileMode for Pipe: " + (String)FileModeToString(mode));
			fp = popen(cmd.Data(), m);
			if (!fp) XLI_THROW("Failed to open pipe: " + cmd);
			stat = -1;
		}
		
		virtual ~PosixPipe()
		{
			if (fp) pclose(fp);
		}
		
		virtual void Flush()
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			fflush(fp);
		}

		virtual void Close()
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			stat = pclose(fp);
			fp = 0;
		}
		
		virtual int GetExitCode() const
		{
			return stat;
		}

		virtual bool AtEnd() const
		{
			return fp == 0;
		}

		virtual bool IsClosed() const
		{
			return fp == 0;
		}

		virtual bool CanRead() const
		{
			return fp != 0;
		}
		virtual bool CanWrite() const
		{
			return fp != 0;
		}

		virtual int Read(void* data, int elmSize, int elmCount)
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			return fread(data, elmSize, elmCount, fp);
		}
		virtual int Write(const void* data, int elmSize, int elmCount)
		{
			if (!fp) XLI_THROW_STREAM_CLOSED;
			return fwrite(data, elmSize, elmCount, fp);
		}
	};
	
	Pipe* Pipe::Create(const CharString& cmd, FileMode mode)
	{
		return new PosixPipe(cmd, mode);
	}
}
