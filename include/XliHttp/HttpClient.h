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
        HttpInvalidState = 1, //is the same as DONE
        HttpUnsent = 2, //send() has not been called yet.
        HttpSent = 3, //send() has been called but a reply has not been delivered.
        HttpHeadersReceived = 4, //send() has been called, and headers and status are available.
        HttpLoading = 5, //downloading; responsetext holds partial data.
        HttpDone = 6, //the operation is complete. or has been aborted
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
    //{TODO} TIMEOUTS GO HERE
    //=======================
    class HttpResponseHandler : public Object
    {
    public:
        virtual void OnResponse(HttpRequest* request) = 0;
    };

    //------------------------------

    class HttpClient : public Object
    {
    public:
        virtual HttpRequest* NewRequest() = 0;
        static HttpClient* Create();       
    };

    //------------------------------

    /* class HttpResponse : public Object */
    /* { */
    /* public: */
    /*     int Status; */
    /*     String ReasonPhrase; */
    /*     HashMap<String,String> Headers; */
    /*     virtual String GetContentString() = 0; */
    /*     virtual Stream* GetContentStream() = 0; */
    /*     static HttpResponse* Create(); */
    /* }; */

    class HttpRequest : public Object
    {
    public:
        HttpResponseHandler* Callback; // managed?

        String Url;
        int Timeout;

        void* Body;
        long BodySizeBytes;

        HashMap<String,String> Headers; // managed?        
        HttpMethodType Method;

        int ResponseStatus;
        String ReasonPhrase;
        HashMap<String,String> ResponseHeaders; // managed?

        static HttpRequest* Create(String url, HttpMethodType method, HttpResponseHandler* callback, const HttpClient* client=NULL);       

        virtual void Send(void* content, long byteLength) = 0;
        virtual void Send(String content) = 0;

        virtual void Abort() = 0;

        virtual String GetContentString() = 0;
        virtual Stream* GetContentStream() = 0;
    };
}

#endif
