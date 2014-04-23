#include <Xli/HashMap.h>
#include <XliHttpClient/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>

namespace Xli
{
    class AHttpRequest : public HttpRequest
    {
    public:
        HttpClient* client;

        HttpRequestState state;

        String url;
        HttpMethod method;
        int timeout;
        HashMap<String,String> headers;

        jobject javaAsyncHandle;
        jobject javaContentHandle;

        bool completedSuccessfully;
        int responseStatus;
        String reasonPhrase;
        Buffer* responseBody;

        HashMap<String,String> responseHeaders;


        AHttpRequest(HttpClient* client, String url, HttpMethod method);

        virtual ~AHttpRequest();

        virtual void CleanHandles();

        virtual String GetMethod() const;
        virtual String GetUrl() const;

        virtual HttpRequestState GetState() const;

        virtual void SetTimeout(int timeout);
        virtual int GetTimeout() const;

        virtual void SetHeader(String key, String value);
        virtual void RemoveHeader(String key);

        virtual int HeadersBegin() const;
        virtual int HeadersEnd() const;
        virtual int HeadersNext(int n) const;
        virtual String GetHeaderKey(int n) const;
        virtual String GetHeaderValue(int n) const;

        virtual void SendASync(void* content, long byteLength);
        virtual void SendASync(String content);
        virtual void SendASync();
        virtual void StartDownload();
        virtual void Abort();

        virtual int GetResponseHeaderCount() const;
        virtual int ResponseHeadersBegin() const;
        virtual int ResponseHeadersEnd() const;
        virtual int ResponseHeadersNext(int n) const;
        virtual String GetResponseHeaderKey(int n) const;
        virtual String GetResponseHeaderValue(int n) const;

        virtual bool TryGetResponseHeader(const String& key, String& result);

        virtual int GetResponseStatus() const;
        virtual DataAccessor* GetResponseBody() const;
    };


    //------ Actions -------//
    class AHttpTimeoutAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        AHttpTimeoutAction(AHttpRequest* request) { Request = request; }
        virtual void Execute()
        {
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestTimeout(Request);
        }
    };

    class AHttpProgressAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        long Position, TotalLength;
        bool LengthKnown;
        HttpTransferDirection Direction;
        AHttpProgressAction(AHttpRequest* request, long position, long totalLength, bool lengthKnown,
                            HttpTransferDirection direction);
        {
            Request = request;
            Position = position;
            TotalLength = totalLength;
            LengthKnown = lengthKnown;
            Direction = direction;
        }

        virtual void Execute()
        {
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestProgress(Request, Position, TotalLength,
                                             LengthKnown, Direction);
        }
    };

    class AHttpErrorAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        int ErrorCode;
        String ErrorMessage;
        AHttpErrorAction(AHttpRequest* request, int errorCode, String errorMessage)
        {
            Request = request;
            ErrorCode = errorCode;
            ErrorMessage = errorMessage;
        }
        virtual void Execute()
        {
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestError(Request);
        }
    };

    class AHttpStateAction : public HttpAction
    {
    public:
        AHttpRequest* Request;
        HttpRequestState State;
        bool CleanJavaObjs;

        AHttpStateAction(AHttpRequest* request, HttpRequestState state, bool cleanJavaObjs = false)
        {
            Request = request;
            State = state;
            CleanJavaObjs = cleanJavaObjs;
        }
        
        virtual void Execute()
        {
            if (CleanJavaObjs) Request->CleanHandles();
            if (State>0) {
                Request->state = State;
                HttpEventHandler* eh = Request->client->GetEventHandler();
                if (eh!=0) eh->OnRequestStateChanged(Request);
            }
        }
    };
}
