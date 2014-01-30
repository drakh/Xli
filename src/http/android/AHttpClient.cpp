#include <Xli/HashMap.h>
#include <XliHttp/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>

namespace Xli
{
	static int HttpInitialized = 0;

    //--------------------------------------------------

    class AHttpRequest : public HttpRequest
    {
    public:
        AHttpRequest(String url, HttpMethods::HttpMethodType method) 
        {
            this->Url = url;
            this->Method = method;
        }

        virtual ~AHttpRequest() 
        {
        }
    };

    HttpRequest* HttpRequest::Create(String url, HttpMethods::HttpMethodType method)
    {
        if (!HttpInitialized)
        {
            PlatformSpecific::AShim::InitDefaultCookieManager();
            HttpInitialized = 1;
        }
        return new AHttpRequest(url, method);
    }
    
    //--------------------------------------------------

    class AHttpClient : public HttpClient
    {
    public:
        AHttpClient() {}
        virtual ~AHttpClient() {}

        virtual void Send(const HttpRequest& req)
        {
            // PlatformSpecific::AShim::SendHttpAsync(req);
        }
    };

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized)
        {
            PlatformSpecific::AShim::InitDefaultCookieManager();
            HttpInitialized = 1;            
        }
        return new AHttpClient();
    }    
};
