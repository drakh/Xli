#include <XliHttp/HttpClient.h>

namespace Xli
{
    class CurlHttpStateAction : public PlatformSpecific::WindowAction
    {
    public:
        CurlHttpRequest* Request;
        HttpRequestState Status;    
        CurlHttpStateAction(CurlHttpRequest* request, HttpRequestState status);
        virtual void Execute();    
    };

    class CurlHttpTimeoutAction : public PlatformSpecific::WindowAction
    {
    public:
        CurlHttpRequest* Request;
        CurlHttpTimeoutAction(CurlHttpRequest* request);
        virtual void Execute();
    };

    class CurlHttpProgressAction : public PlatformSpecific::WindowAction
    {
    public:
        CurlHttpRequest* Request;
        long Position;
        long TotalLength;
        bool LengthKnown;

        CurlHttpProgressAction(CurlHttpRequest* request, long position, long totalLength, bool lengthKnown);
        virtual void Execute();
    };

    class CurlHttpErrorAction : public PlatformSpecific::WindowAction
    {
    public:
        CurlHttpRequest* Request;
        int ErrorCode;
        String ErrorMessage;

        CurlHttpErrorAction(CurlHttpRequest* request, int errorCode, String errorMessage);
        virtual void Execute();
    };

}
