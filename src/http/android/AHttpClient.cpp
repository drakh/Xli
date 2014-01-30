#include <Xli/HashMap.h>
#include <XliHttp/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>

namespace Xli
{
	static int HttpInitialized = 0;

    //--------------------------------------------------

    // class AHttpResponse : public HttpResponse
    // {
    // public:
    //     AHttpResponse() 
    //     {
    //     }

    //     virtual ~AHttpResponse() 
    //     {
    //     }
    // };

    // HttpResponse* HttpResponse::Create()
    // {
    //     if (!HttpInitialized)
    //     {
    //         PlatformSpecific::AShim::InitDefaultCookieManager();
    //         HttpInitialized = 1;
    //     }
    //     return new AHttpResponse();
    // }

    //--------------------------------------------------

    PlatformSpecific::AHttpRequest::AHttpRequest(String url, HttpMethods::HttpMethodType method, HttpResponseHandler* callback) 
    {
        this->Url = url;
        this->Method = method;
        this->OutboundPayloadType = HttpMethods::STRING;
        this->ReturnPayloadType = HttpMethods::STRING;
        this->Callback = callback;
    }

    PlatformSpecific::AHttpRequest::~AHttpRequest() 
    {
    }

    HttpRequest* HttpRequest::Create(String url, HttpMethods::HttpMethodType method,
                                     HttpResponseHandler* callback)
    {
        if (!HttpInitialized)
        {
            PlatformSpecific::AShim::InitDefaultCookieManager();
            HttpInitialized = 1;
        }
        return new PlatformSpecific::AHttpRequest(url, method, callback);
    }
    
    //--------------------------------------------------

    class AHttpClient : public HttpClient
    {
    public:
        AHttpClient() {}
        virtual ~AHttpClient() {}

        virtual void Send(const HttpRequest& req)
        {
            PlatformSpecific::AShim::SendHttpAsync((PlatformSpecific::AHttpRequest&)req);
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
