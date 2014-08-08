//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

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
            OutWriter = new TextWriter(ManagePtr(new File(stdout, FileFlagsCanWrite | FileFlagsIgnoreWriteErrors)));
            ErrWriter = new TextWriter(ManagePtr(new File(stderr, FileFlagsCanWrite | FileFlagsIgnoreWriteErrors)));
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
