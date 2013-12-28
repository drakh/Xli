#ifndef __XLI_CONSOLE_H__
#define __XLI_CONSOLE_H__

#include <Xli/TextReader.h>
#include <Xli/TextWriter.h>

namespace Xli
{
    /**
        \ingroup XliCoreIO
    */
    class Console
    {
    public:
        static void Init();
        static void Done();
    };

    /**
        \ingroup XliCoreIO
    */
    class StdOutAccessor
    {
    public:
        TextWriter* operator ->();
        operator TextWriter*();
    };

    /**
        \ingroup XliCoreIO
    */
    class StdErrAccessor
    {
    public:
        TextWriter* operator ->();
        operator TextWriter*();
    };

    /**
        \ingroup XliCoreIO
    */
    class StdInAccessor
    {
    public:
        TextReader* operator ->();
        operator TextReader*();
    };

    /**
        \addtogroup XliCoreIO
        @{
    */

    extern StdOutAccessor Out;
    extern StdErrAccessor Err;
    extern StdInAccessor In;

    template <typename T> 
    XLI_INLINE void Print(const T& t)
    {
        Out->Write(t);
    }

    template <typename T> 
    XLI_INLINE void PrintLine(const T& t)
    {
        Out->WriteLine(t);
    }

    XLI_INLINE void PrintLine()
    {
        Out->Write('\n');
    }

    template <typename T> 
    void ErrorPrint(const T& t)
    {
        Err->Write(t);
    }

    template <typename T> 
    void ErrorPrintLine(const T& t)
    {
        Err->WriteLine(t);
    }

    XLI_INLINE void ErrorPrintLine()
    {
        Err->Write('\n');
    }

    /** @} */
}


#endif
