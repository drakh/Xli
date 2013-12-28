#include <Xli/Console.h>
#include <Xli/File.h>
#include <cstdlib>

namespace Xli
{
    static TextWriter* OutWriter = 0;
    static TextWriter* ErrWriter = 0;
    static TextReader* InReader = 0;

    static int InitCount = 0;

    void Console::Init()
    {
        if (!InitCount)
        {
            OutWriter = new TextWriter(ManagePtr(new File(stdout, FileFlagsCanWrite | FileFlagsIgnoreReadWriteErrors)));
            ErrWriter = new TextWriter(ManagePtr(new File(stderr, FileFlagsCanWrite | FileFlagsIgnoreReadWriteErrors)));
            InReader = new TextReader(ManagePtr(new File(stdin, FileFlagsCanRead)));
        }

        InitCount++;
    }

    void Console::Done()
    {
        InitCount--;

        if (!InitCount)
        {
            OutWriter->GetStream()->Flush();
            ErrWriter->GetStream()->Flush();
            delete OutWriter;
            delete ErrWriter;
            delete InReader;
            OutWriter = 0;
            ErrWriter = 0;
            InReader = 0;
        }
        else if (InitCount < 0)
        {
            XLI_THROW_BAD_DELETE;
        }
    }

    static void AssertInit()
    {
        if (!InitCount)
        {
            Console::Init();
            atexit(Console::Done);
        }
    }

    TextWriter* StdOutAccessor::operator ->()
    {
        AssertInit();
        return OutWriter;
    }

    StdOutAccessor::operator TextWriter*()
    {
        AssertInit();
        return OutWriter;
    }

    TextWriter* StdErrAccessor::operator ->()
    {
        AssertInit();
        return ErrWriter;
    }

    StdErrAccessor::operator TextWriter*()
    {
        AssertInit();
        return ErrWriter;
    }

    TextReader* StdInAccessor::operator ->()
    {
        AssertInit();
        return InReader;
    }

    StdInAccessor::operator TextReader*()
    {
        AssertInit();
        return InReader;
    }

    StdOutAccessor Out;
    StdErrAccessor Err;
    StdInAccessor In;
}
