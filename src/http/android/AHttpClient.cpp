#include <Xli/HashMap.h>
#include <XliHttp/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>

extern "C"
{
    void JNICALL XliJ_HttpCallback (JNIEnv *env , jobject obj, jobject body,
                                    jlong handlerPointer) 
    {        
        if (body) //this shouldnt stop response 
        {
            env->NewGlobalRef(body);
            Xli::PlatformSpecific::AStream* stream = new Xli::PlatformSpecific::AStream(Xli::PlatformSpecific::AStream::READ, body);

            Xli::HttpResponse* response = Xli::HttpResponse::Create();
            response->Body = stream;

            if (handlerPointer)
            {
                ((Xli::HttpResponseHandler*)((void*)handlerPointer))->OnResponse(response);
            } else {
                LOGE("No callback pointer error");
            }
        }                
    }
}

namespace Xli
{
	static int HttpInitialized = 0;

    static void InitAndroidHttp()
    {
        if (!HttpInitialized)
        {
            PlatformSpecific::AShim::InitDefaultCookieManager();            
            static JNINativeMethod nativeFuncs[] = {
                {(char* const)"XliJ_HttpCallback", (char* const)"(Ljava/lang/Object;J)V",
                 (void *)&XliJ_HttpCallback},
            };
            bool result = PlatformSpecific::AShim::RegisterNativeFunctions(nativeFuncs, 1);
            if (!result) LOGE("Could not register http callbacks");
            HttpInitialized = 1;
        }
    }

    //--------------------------------------------------

    class AHttpResponse: public HttpResponse
    {
    public:
        AHttpResponse() {}
        virtual ~AHttpResponse() {}
    };

    HttpResponse* HttpResponse::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpResponse();
    }

    //--------------------------------------------------

    class AHttpRequest : public HttpRequest
    {
    public:
        AHttpRequest(String url, HttpMethods::HttpMethodType method, 
                     HttpResponseHandler* callback) 
        {
            this->Url = url;
            this->Method = method;
            this->Callback = callback;
        }

        virtual ~AHttpRequest() 
        {
        }
    };

    HttpRequest* HttpRequest::Create(String url, HttpMethods::HttpMethodType method,
                                     HttpResponseHandler* callback)
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpRequest(url, method, callback);
    }
    
    //--------------------------------------------------

    class AHttpClient : public HttpClient
    {
    public:
        AHttpClient() {}
        virtual ~AHttpClient() {}

        virtual void Send(const HttpRequest& req)
        {
            PlatformSpecific::AShim::SendHttpAsync(req);
        }
    };

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpClient();
    }    
};
