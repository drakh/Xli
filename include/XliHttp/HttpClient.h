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

   class HttpClient : public Object
   {
   public:
       //-----------------
       static HttpClient* Create();       
   };

   class HttpResponse : public Object
   {
   public:
       int Status; //implement me
       String ReasonPhrase; //implement me
       HashMap<String,String> Headers;
       Stream* Body; // test me
       static HttpResponse* Create();
   };

   class HttpResponseHandler : public Object
   {
   public:
       virtual void OnResponse(HttpResponse* response) = 0;
   };

   class HttpRequest : public Object
   {
   public:
       HttpResponseHandler* Callback;       
       String Url;
       String Mime; //implement me
       int Timeout; //implement me
       Stream* Body; //implement me
       HashMap<String,String> Headers; // managed?
       HttpMethodType Method;
       static HttpRequest* Create(String url, HttpMethodType method, HttpResponseHandler* callback, const HttpClient* client=NULL);       
       virtual void Send() = 0;
       virtual void Abort() = 0;
   };
}

#endif
