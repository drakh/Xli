#ifndef __XLI_HTTP_HTTP_CLIENT_H__
#define __XLI_HTTP_HTTP_CLIENT_H__

#include <Xli/String.h>
#include <Xli/Stream.h>
#include <Xli/HashMap.h>
#include <Xli/MutexQueue.h>

//{TODO} Need to add a cross platform error code for http.

namespace Xli
{   
    enum HttpMethod
    {
        HttpMethodUnknown = 0,
        HttpMethodGet = 1,
        HttpMethodPost = 2,
        HttpMethodOptions = 3,
        HttpMethodHead = 4,
        HttpMethodPut = 5,
        HttpMethodDelete = 6,
        HttpMethodTrace = 7,
    };
    enum HttpRequestState
    {
        HttpRequestStateInvalid = 0,
        HttpRequestStateUnsent = 1, //send() has not been called yet.
        HttpRequestStateSent = 2, //send() has been called but a reply has not been delivered.
        HttpRequestStateHeadersReceived = 3, //headers and status are available.
        HttpRequestStateLoading = 4, //downloading; responsetext holds partial data.
        HttpRequestStateDone = 5, //the operation is complete. or has been aborted
    };

    inline String HttpMethodToString(HttpMethod method)
    {
        switch(method)
        {
        case HttpMethodGet: return String("GET");
        case HttpMethodPost: return String("POST");
        case HttpMethodOptions: return String("OPTIONS");
        case HttpMethodHead: return String("HEAD");
        case HttpMethodPut: return String("PUT");
        case HttpMethodDelete: return String("DELETE");
        case HttpMethodTrace: return String("TRACE");
        default: return String("INVALID");
        };
    }
    inline HttpMethod StringToHttpMethod(String method)
    {
        method = method.ToUpper();
        if (method == "GET"){
            return HttpMethodGet;
        } else if (method == "POST") {
            return HttpMethodPost;
        } else if (method == "OPTIONS") {
            return HttpMethodOptions;
        } else if (method == "HEAD") {
            return HttpMethodHead;
        } else if (method == "PUT") {
            return HttpMethodPut;
        } else if (method == "DELETE") {
            return HttpMethodDelete;
        } else if (method == "TRACE") {
            return HttpMethodTrace;
        } else {
            return HttpMethodUnknown;
        }
    }           

    class HttpRequest;

    //=======================
    // Callbacks
    //=======================
    class HttpStateChangedHandler : public Object
    {
    public:
        virtual void OnResponse(HttpRequest* request, HttpRequestState state) = 0;
    };
    class HttpProgressHandler : public Object
    {
    public:
        virtual void OnResponse(HttpRequest* request, long position, long total, bool totalKnown) = 0;
    };
    class HttpErrorHandler : public Object
    {
    public:
        virtual void OnResponse(HttpRequest* request, int errorCode, String errorMessage) = 0;
    };
    class HttpTimeoutHandler : public Object
    {
    public:
        virtual void OnResponse(HttpRequest* request) = 0;
    };
    class HttpStringPulledHandler : public Object
    {
    public:
        virtual void OnResponse(HttpRequest* request, String content) = 0;
    };
    class HttpArrayPulledHandler : public Object
    {
    public:
        virtual void OnResponse(HttpRequest* request, void* content, long byteLength) = 0;
    };

    //------------------------------

    class HttpRequest : public Object
    {
    public:
        virtual HttpRequestState GetStatus() const = 0;

        virtual void SetHeader(String key, String value) = 0;
        virtual void RemoveHeader(String key) = 0;
        virtual int HeadersBegin() const = 0;
        virtual int HeadersEnd() const = 0;
        virtual int HeadersNext(int n) const = 0;
        virtual String GetHeaderKeyN(int n) const = 0;
        virtual String GetHeaderValueN(int n) const = 0;
        virtual String GetHeadersAsString() const = 0;

        virtual int GetResponseHeaderCount() const = 0;
        virtual int ResponseHeadersBegin() const = 0;
        virtual int ResponseHeadersEnd() const = 0;
        virtual int ResponseHeadersNext(int n) const = 0;
        virtual String GetResponseHeaderKeyN(int n) const = 0;        
        virtual String GetResponseHeaderValueN(int n) const = 0;        
        virtual String GetResponseHeader(String key) = 0;
        /* {TODO} virtual String GetResponseHeadersAsString() const = 0; */ 

        virtual void SetMethod(HttpMethod method) = 0;
        virtual void SetMethodFromString(String method) = 0;
        virtual HttpMethod GetMethod() const = 0;

        virtual void SetUrl(String url) = 0;
        virtual String GetUrl() const = 0;

        virtual void SetTimeout(int timeout) = 0;
        virtual int GetTimeout() const = 0;

        virtual int GetResponseStatus() const = 0;
        virtual String GetReasonPhrase() const = 0;

        virtual void SetStateChangedCallback(HttpStateChangedHandler* callback) = 0;
        virtual void SetProgressCallback(HttpProgressHandler* callback) = 0;
        virtual void SetTimeoutCallback(HttpTimeoutHandler* callback) = 0;
        virtual void SetErrorCallback(HttpErrorHandler* callback) = 0;
        virtual void SetStringPulledCallback(HttpStringPulledHandler* callback) = 0;
        virtual void SetArrayPulledCallback(HttpArrayPulledHandler* callback) = 0;

        virtual void SendASync(void* content, long byteLength) = 0;
        virtual void SendASync(String content) = 0;
        virtual void SendASync() = 0;

        virtual void Abort() = 0;

        virtual void PullContentString() = 0;
        virtual void PullContentArray() = 0;
    };

    class HttpAction : public Object
    {
    public:
        virtual void Execute() = 0;
    };
    
    class HttpClient : public Object
    { 
    private:
        MutexQueue<HttpAction*> actionQueue;
    public:
        virtual HttpRequest* CreateRequest() = 0;
        virtual void Update() {};
        virtual void EnqueueAction(HttpAction* action) {};
        static HttpClient* Create();
    };
}

#endif
