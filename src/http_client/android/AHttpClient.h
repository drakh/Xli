#include <Xli/HashMap.h>
#include <XliHttpClient/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>

namespace Xli
{
    class AHttpRequest : public HttpRequest
    {
    private:
        String url;
        int timeout;
    public:
        Managed< HttpStateChangedHandler > stateChangedCallback;
        Managed< HttpProgressHandler > progressCallback;
        Managed< HttpTimeoutHandler > timeoutCallback;
        Managed< HttpErrorHandler > errorCallback;
        Managed< HttpStringPulledHandler > stringPulledCallback;
        Managed< HttpArrayPulledHandler > arrayPulledCallback;
        HashMap<String,String> headers;
        HashMap<String,String> responseHeaders;
        HttpRequestState status;
        HttpMethod method;
        int responseStatus;
        String reasonPhrase;
        jobject javaAsyncHandle;
        jobject javaContentHandle;
        String cachedContentString;
        void* cachedContentArray;
        long cachedContentArrayLength;
        bool completedSuccessfully;
        bool arrayBody;
        HttpClient* client;

        AHttpRequest(HttpClient* client);
        AHttpRequest(HttpClient* client, String url, HttpMethod method);
        virtual ~AHttpRequest();
        virtual HttpRequestState GetStatus() const;
        virtual void SetHeader(String key, String value);
        virtual void RemoveHeader(String key);
        virtual int HeadersBegin() const;
        virtual int HeadersEnd() const;
        virtual int HeadersNext(int n) const;
        virtual String GetHeaderKeyN(int n) const;
        virtual String GetHeaderValueN(int n) const;
        virtual String GetHeadersAsString() const;
        virtual int GetResponseHeaderCount() const;
        virtual int ResponseHeadersBegin() const;
        virtual int ResponseHeadersEnd() const;
        virtual int ResponseHeadersNext(int n) const;
        virtual String GetResponseHeaderKeyN(int n) const;
        virtual String GetResponseHeaderValueN(int n) const;
        virtual String GetResponseHeader(String key);
        virtual void SetMethod(HttpMethod method);
        virtual void SetMethodFromString(String method);
        virtual HttpMethod GetMethod() const;
        virtual void SetUrl(String url);
        virtual String GetUrl() const;
        virtual void SetTimeout(int timeout);
        virtual int GetTimeout() const;
        virtual int GetResponseStatus() const;
        virtual String GetReasonPhrase() const;
        virtual void SetStateChangedCallback(HttpStateChangedHandler* callback);
        virtual void SetProgressCallback(HttpProgressHandler* callback);
        virtual void SetTimeoutCallback(HttpTimeoutHandler* callback);
        virtual void SetErrorCallback(HttpErrorHandler* callback);
        virtual void SetStringPulledCallback(HttpStringPulledHandler* callback);
        virtual void SetArrayPulledCallback(HttpArrayPulledHandler* callback);
        virtual void SendASync(void* content, long byteLength);
        virtual void SendASync(String content);
        virtual void SendASync();
        virtual void Abort();
        virtual void PullContentString();
        virtual void PullContentArray();
    };

    class AHttpTimeoutAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        AHttpTimeoutAction(AHttpRequest* request);
        virtual void Execute();
    };

    class AHttpProgressAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        long Position;
        long TotalLength;
        bool LengthKnown;
        AHttpProgressAction(AHttpRequest* request, long position, long totalLength, bool lengthKnown);
        virtual void Execute();
    };

    class AHttpErrorAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        int ErrorCode;
        String ErrorMessage;
        AHttpErrorAction(AHttpRequest* request, int errorCode, String errorMessage);
        virtual void Execute();
    };

    class AHttpStateAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        HttpRequestState Status;
        bool CleanJavaObjs;
        AHttpStateAction(AHttpRequest* request, HttpRequestState status, bool cleanJavaObjs = false);
        virtual void Execute();
    };
}
