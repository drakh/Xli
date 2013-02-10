#include <Xli/Console.h>
#import <Foundation/NSObjCRuntime.h>

namespace Xli
{
    class NSLogStream: public Stream
    {
        Array<char> buf;
        
    public:
        virtual bool CanWrite() const
        {
            return true;
        }
        
        virtual int Write(const void* src, int elmSize, int elmCount)
        {
            for (int i = 0; i < elmCount; i++)
            {
                char c = ((char*)src)[i];
                
                if (c == '\n')
                {
                    buf.Add('\0');
                    NSLog(@"%@", [NSString stringWithUTF8String:buf.Data()]);
                    buf.Clear();
                    continue;
                }
                else if (c == '\t')
                {
                    buf.Add("    ", 4 - buf.Length() % 4);
                    continue;
                }
                else if (c == '\r')
                {
                    continue;
                }
                
                buf.Add(c);
            }
            
            return elmCount;
        }
        
        virtual void Flush()
        {
            if (buf.Length() > 0)
            {
                buf.Add('\0');
                NSLog(@"%@", [NSString stringWithUTF8String:buf.Data()]);
                buf.Clear();
            }
        }
    };
}

using namespace Xli;

void XliInit_iOS()
{
    Managed<Stream> log = new NSLogStream();
    Out->SwitchStream(log);
    Err->SwitchStream(log);
}
