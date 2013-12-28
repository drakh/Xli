#include <Xli/TextStream.h>

namespace Xli
{
    TextStream::TextStream(int initCapacity)
        : ArrayStream(initCapacity) 
    {
    }

    String TextStream::GetText() const 
    { 
        return String((const char*)buf.Data(), GetLength()); 
    }
}
