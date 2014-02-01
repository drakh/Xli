#include <Xli/HashMap.h>
#include <XliHttp/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>

extern Xli::Window* GlobalWindow;

namespace Xli
{

    namespace PlatformSpecific
    {
        class AHttpResponseHandlerAction : public AWindowAction
        {    
        public:
            Managed<HttpResponseHandler> Handler;
            Managed<HttpResponse> Response;

            AHttpResponseHandlerAction(HttpResponseHandler* handler, HttpResponse* response)
            {
                this->Handler = handler;
                this->Response = response;
            }
            virtual ~AHttpResponseHandlerAction() {}
            virtual void Execute()
            {
                this->Handler.Get()->OnResponse(this->Response.Get());
            }
        };
    }
}

extern "C"
{
    void JNICALL XliJ_HttpCallback (JNIEnv *env , jobject obj, jobject body,
                                    jobjectArray headers, jlong handlerPointer) 
    {       
        if (handlerPointer)
        {
            Xli::HttpResponse* response = Xli::HttpResponse::Create();

            if (body)
            {
                env->NewGlobalRef(body);
                Xli::PlatformSpecific::AStream* stream = new Xli::PlatformSpecific::AStream(Xli::PlatformSpecific::AStream::READ, body);            
                response->Body = stream;
            } else {
                response->Body = 0;
            }

            if (headers)
            {
                int headerCount = (env->GetArrayLength(headers) / 2);
                int index = 0;
                jstring jkey;
                jstring jval;
                char const* ckey;
                char const * cval;
                for (int i=0; i<headerCount; i++)
                {
                    index = i * 2;

                    jkey = (jstring) env->GetObjectArrayElement(headers, index);
                    jval = (jstring) env->GetObjectArrayElement(headers, (index + 1));

                    //{TODO} again -null- is an issue
                    if (!jkey) { 
                        ckey = "-null-"; 
                    } else { 
                        ckey = env->GetStringUTFChars(jkey, NULL); 
                    }
                    if (!jval) { 
                        cval = "-null-"; 
                    } else { 
                        cval = env->GetStringUTFChars(jval, NULL); 
                    }

                    response->Headers.Add(ckey,cval);
                    env->ReleaseStringUTFChars(jkey, ckey);
                    env->ReleaseStringUTFChars(jval, cval);
                }
            } else {
                headers = 0;
            }
            env->DeleteLocalRef(headers);
                     
            Xli::CTEvent* event = new Xli::CTEvent();
            event->CTType = Xli::CTActionEvent;
            event->Code = -1;
            event->Payload = (void*)(new Xli::PlatformSpecific::AHttpResponseHandlerAction(((Xli::HttpResponseHandler*)((void*)handlerPointer)), response));
            GlobalWindow->EnqueueCrossThreadEvent(event);
            //((Xli::HttpResponseHandler*)((void*)handlerPointer))->OnResponse(response);
        } else {
            LOGE("No callback pointer error");
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
                {(char* const)"XliJ_HttpCallback", (char* const)"(Ljava/lang/Object;[Ljava/lang/String;J)V",
                 (void *)&XliJ_HttpCallback},
            };
            bool result = PlatformSpecific::AShim::RegisterNativeFunctions(nativeFuncs, 1);
            if (!result) LOGE("*** Could not register http callbacks");
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
    private:
        jobject javaConnectionHandle;
    public:
        AHttpRequest(String url, HttpMethodType method, 
                     HttpResponseHandler* callback) 
        {
            this->Url = url;
            this->Method = method;
            this->Mime = NULL;
            this->Timeout = 0;
            this->Callback = callback;
            this->javaConnectionHandle = 0;
        }

        virtual ~AHttpRequest() 
        {
        }

        virtual void Send()
        {
            javaConnectionHandle = PlatformSpecific::AShim::SendHttpAsync(this);
        }
        
        virtual void Abort()
        {
            if (javaConnectionHandle != 0)
                PlatformSpecific::AShim::AbortAsyncConnection(javaConnectionHandle);
            javaConnectionHandle = 0;
        }
    };

    HttpRequest* HttpRequest::Create(String url, HttpMethodType method,
                                     HttpResponseHandler* callback, const HttpClient* client)
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
    };

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpClient();
    }
};
