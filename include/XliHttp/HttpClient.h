#ifndef __XLI_HTTP_HTTP_CLIENT_H__
#define __XLI_HTTP_HTTP_CLIENT_H__

#include <Xli/String.h>
#include <Xli/Stream.h>
#include <Xli/HashMap.h>


namespace Xli
{   
    enum HttpMethodType
    {
        HttpGetMethod = 0,
        HttpPostMethod = 1,
        HttpOptionsMethod = 2,
        HttpHeadMethod = 3,
        HttpPutMethod = 4,
        HttpDeleteMethod = 5,
        HttpTraceMethod = 6,
        HttpInvalidMethod = 7,
    };
    enum HttpRequestState
    {
        HttpInvalidState = 999,
        HttpUnsent = 1, //send() has not been called yet.
        HttpSent = 2, //send() has been called but a reply has not been delivered.
        HttpHeadersReceived = 3, //headers and status are available.
        HttpLoading = 4, //downloading; responsetext holds partial data.
        HttpDone = 5, //the operation is complete. or has been aborted
    };

    inline String HttpMethodToString(HttpMethodType method)
    {
        switch(method)
        {
        case HttpGetMethod: return String("GET");
        case HttpPostMethod: return String("POST");
        case HttpOptionsMethod: return String("OPTIONS");
        case HttpHeadMethod: return String("HEAD");
        case HttpPutMethod: return String("PUT");
        case HttpDeleteMethod: return String("DELETE");
        case HttpTraceMethod: return String("TRACE");
        default: return String("INVALID");
        };
    }
    inline HttpMethodType StringToHttpMethod(String method)
    {
        method = method.ToUpper();
        if (method == "GET"){
            return HttpGetMethod;
        } else if (method == "POST") {
            return HttpPostMethod;
        } else if (method == "OPTIONS") {
            return HttpOptionsMethod;
        } else if (method == "HEAD") {
            return HttpHeadMethod;
        } else if (method == "PUT") {
            return HttpPutMethod;
        } else if (method == "DELETE") {
            return HttpDeleteMethod;
        } else if (method == "TRACE") {
            return HttpTraceMethod;
        } else {
            return HttpInvalidMethod;
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
        static HttpRequest* Create();

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

        virtual void SetMethod(HttpMethodType method) = 0;
        virtual void SetMethodFromString(String method) = 0;
        virtual HttpMethodType GetMethod() const = 0;

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

        virtual void Send(void* content, long byteLength) = 0;
        virtual void Send(String content) = 0;
        virtual void Send() = 0;

        virtual void Abort() = 0;

        virtual void PullContentString() = 0;
        /* virtual String GetContentString() = 0; */

        virtual void PullContentArray() = 0;
        /* virtual void* GetContentArray() = 0; */
        /* virtual long GetContentArrayLength() = 0; */
    };
}

#endif
