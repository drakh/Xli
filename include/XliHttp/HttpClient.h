#ifndef __XLI_HTTP_HTTP_CLIENT_H__
#define __XLI_HTTP_HTTP_CLIENT_H__

#include <Xli/String.h>
#include <Xli/Stream.h>
#include <Xli/HashMap.h>


namespace Xli
{
   namespace HttpMethods
   {
       enum HttpMethodType
       {
           GET = 0,
           POST = 1,
           OPTIONS = 2,
           HEAD = 3,
           PUT = 4,
           DELETE = 5,
           TRACE = 6,
           INVALID = 7,
       };
       inline
       String MethodToString(HttpMethodType method)
       {
           switch(method)
           {
           case GET: return String("GET");
           case POST: return String("POST");
           case OPTIONS: return String("OPTIONS");
           case HEAD: return String("HEAD");
           case PUT: return String("PUT");
           case DELETE: return String("DELETE");
           case TRACE: return String("TRACE");
           default: return String("INVALID");
           };
       }
       inline
       HttpMethodType StringToMethod(String method)
       {
           if (method == "GET"){
               return GET;
           } else if (method == "POST") {
               return POST;
           } else if (method == "OPTIONS") {
               return OPTIONS;
           } else if (method == "HEAD") {
               return HEAD;
           } else if (method == "PUT") {
               return PUT;
           } else if (method == "DELETE") {
               return DELETE;
           } else if (method == "TRACE") {
               return TRACE;
           } else {
               return INVALID;
           }
       }        
   }

   class HttpResponse : public Object
   {
   public:       
       int Status;
       String ReasonPhrase;
       HashMap<String,String> Headers;
       Stream* Body;
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
       String Url;
       HttpMethods::HttpMethodType Method;
       HashMap<String,String> Headers;
       String Mime;
       HttpResponseHandler* Callback;
       //-----------------
       static HttpRequest* Create(String url, HttpMethods::HttpMethodType method, HttpResponseHandler* callback);
   };

   class HttpClient : public Object
   {
   public:
       //-----------------
       static HttpClient* Create();
       virtual void Send(const HttpRequest& req) = 0;
   };
}

#endif
