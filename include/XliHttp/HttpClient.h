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
        HttpMethods::HttpMethodType GetMethod() const { return this->method; }
        inline
        String GetMethodAsString() const { return HttpMethods::MethodToString(this->method); }
	};

	class HttpResponse: public Object
	{
    protected:
        bool valid;
	public:
        virtual bool IsValid() const = 0;
        virtual String GetHeader(const String& key) = 0;
        virtual int GetResponseCode() const = 0;
		Stream* Payload;
	};

	class HttpClient: public Object
	{
	public:
		static void Init();
		static void Done();
		static HttpClient* Create();
		virtual HttpResponse* Send(const String& uri, const HttpRequest& req) = 0;
	};



    class HttpClientWrapper : public Object
    {
    public:
        static HttpClientWrapper * Create();
        void Open(String method, String url);
        void Send(String data);
        void SetRequestHeader(String header, String value);
        unsigned short GetStatus();
        String GetResponseText();
        String GetResponseHeader(String header);
        String GetAllResponseHeaders();
        String GetStatusText();
    
    private:
        String method;
        String url;
        HttpClient * client;
        HttpRequest * request;
        HttpResponse * response;
    };

}

#endif
