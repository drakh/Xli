#ifndef __XLI_CONSOLE_H__
#define __XLI_CONSOLE_H__

#include <Xli/TextReader.h>
#include <Xli/TextWriter.h>

namespace Xli
{
	/**
		\ingroup IO
	*/
	class Console
	{
	public:
		static void Init();
		static void Done();
	};

	/**
		\ingroup IO
	*/
	class StdOutAccessor
	{
	public:
		TextWriter* operator ->();
		operator TextWriter*();
	};

	/**
		\ingroup IO
	*/
	class StdErrAccessor
	{
	public:
		TextWriter* operator ->();
		operator TextWriter*();
	};

	/**
		\ingroup IO
	*/
	class StdInAccessor
	{
	public:
		TextReader* operator ->();
		operator TextReader*();
	};

	/**
		\addtogroup IO
		@{
	*/

	extern StdOutAccessor Out;
	extern StdErrAccessor Err;
	extern StdInAccessor In;

	template <typename T> static void Print(const T& t)
	{
		Out->Write(t);
	}

	template <typename T> static void PrintLine(const T& t)
	{
		Out->WriteLine(t);
	}

	static void PrintLine()
	{
		Out->Write('\n');
	}

	template <typename T> void ErrorPrint(const T& t)
	{
		Err->Write(t);
	}

	template <typename T> void ErrorPrintLine(const T& t)
	{
		Err->WriteLine(t);
	}

	static void ErrorPrintLine()
	{
		Err->Write('\n');
	}

	/** @} */
}


#endif
