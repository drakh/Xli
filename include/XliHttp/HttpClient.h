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
            };
        }
    }

	class HttpRequest: public Object
	{
    private:
        HttpMethods::HttpMethodType method;
	public:
        static HttpRequest* Create();
        virtual void SetHeader(const String& key, const String& val) = 0;
        virtual const HashMap<String,String>& GetHeaders() const = 0;
        virtual void SetPayload(Stream* payload) = 0;
        virtual bool HasPayload() const = 0;
        virtual Stream* GetPayload() const = 0;
        inline
        void SetMethod(HttpMethods::HttpMethodType method) { this->method = method; }
        inline
        HttpMethods::HttpMethodType GetMethod() { return this->method; }
        inline
        String GetMethodAsString() const { return HttpMethods::MethodToString(this->method); }
	};

	class HttpResponse: public Object
	{
	public:
        bool valid;
        virtual String GetHeader(const String& key) = 0;
		Stream* payload;
        String method;
	};

	class HttpClient: public Object
	{
	public:
		static void Init();
		static void Done();
		static HttpClient* Create();
		virtual HttpResponse* Send(const String& uri, const HttpRequest& req) = 0;
	};
}

#endif
