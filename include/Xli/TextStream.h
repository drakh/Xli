#ifndef __XLI_TEXT_STREAM_H__
#define __XLI_TEXT_STREAM_H__

#include <Xli/ArrayStream.h>

namespace Xli
{
    /**
        \ingroup XliIO
    */
    class TextStream: public ArrayStream
    {
    public:
        TextStream(int initCapacity = 1024);
        String GetText() const;
    };
}

#endif
