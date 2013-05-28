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
	class OutAccessor
	{
	public:
		TextWriter* operator ->();
		operator TextWriter*();
	};

	/**
		\ingroup IO
	*/
	class ErrAccessor
	{
	public:
		TextWriter* operator ->();
		operator TextWriter*();
	};

	/**
		\ingroup IO
	*/
	class InAccessor
	{
	public:
		TextReader* operator ->();
		operator TextReader*();
	};

	/**
		\addtogroup IO
		@{
	*/

	extern OutAccessor Out;
	extern ErrAccessor Err;
	extern InAccessor In;

	template <typename T> static inline void Print(const T& t)
	{
		Out->Write(t);
	}
	template <typename T> static inline void PrintLine(const T& t)
	{
		Out->WriteLine(t);
	}
	static inline void PrintLine()
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
	static inline void ErrorPrintLine()
	{
		Err->Write('\n');
	}

	/** @} */
}


#endif
