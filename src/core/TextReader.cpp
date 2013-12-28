#include <Xli/TextReader.h>
#include <Xli/File.h>

namespace Xli
{
    TextReader::TextReader(Stream* stream)
        : StreamReader(stream)
    {
    }

    String TextReader::Read(int len)
    {
        String s = String::Create(len);
        stream->ReadSafe(s.Data(), 1, len);
        return s;
    }

    String TextReader::ReadAll()
    {
        if (stream->CanSeek())
            return Read(stream->GetLength());
        
        Array<char> str;

        char buf[1024];
        int len = 0;
        
        while ((len = stream->Read(buf, 1, 1024)))
            str.Add(buf, len);
        
        return String(str.Data(), str.Length());
    }

    char TextReader::ReadChar()
    {
        char c;
        stream->ReadSafe(&c, 1, 1);
        return c;
    }

    String TextReader::ReadTo(char terminal)
    {
        Array<char> s;
        char c;
        
        while (stream->Read(&c, 1, 1) == 1)
        {
            s.Add(c);
            
            if (c == terminal)
                break;
        }

        return String(s.Data(), s.Length());
    }

    String TextReader::ReadLine()
    {
        return ReadTo('\n');
    }

    bool TextReader::AtEndOfFile()
    {
        return stream->AtEnd();
    }
}
