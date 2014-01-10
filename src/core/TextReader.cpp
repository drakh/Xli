#include <Xli/TextReader.h>
#include <Xli/File.h>

namespace Xli
{
    TextReader::TextReader(Stream* stream)
        : StreamReaderBase(stream)
    {
    }

    String TextReader::Read(int len)
    {
        String s = String::Create(len);
        _stream->ReadSafe(s.Data(), 1, len);
        return s;
    }

    String TextReader::ReadAll()
    {
        if (_stream->CanSeek())
            return Read(_stream->GetLength());
        
        Array<char> str;

        char buf[1024];
        int len = 0;
        
        while ((len = _stream->Read(buf, 1, 1024)))
            str.Add(buf, len);
        
        return String(str.Data(), str.Length());
    }

    char TextReader::ReadChar()
    {
        char c;
        _stream->ReadSafe(&c, 1, 1);
        return c;
    }

    String TextReader::ReadTo(char terminal)
    {
        Array<char> s;
        char c;
        
        while (_stream->Read(&c, 1, 1) == 1)
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
        return _stream->AtEnd();
    }
}
