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
            AHttpRequest* Response;

            AHttpResponseHandlerAction(HttpResponseHandler* handler, AHttpRequest* request)
            {
                this->Handler = handler;
                this->Response = request;
            }
            virtual ~AHttpResponseHandlerAction() {}
            virtual void Execute()
            {                
                AJniHelper jni;
                this->Handler.Get()->OnResponse(this->Response.Get());
            }
        };
    }
}

extern "C"
{
    void JNICALL XliJ_HttpCallback (JNIEnv *env , jobject obj, jobject body,
                                    jobjectArray headers, jint responseCode,
                                    jstring responseMessage, jlong requestPointer)
    {       
        if (requestPointer)
        {
            Xli::AHttpRequest* response = ((Xli::HttpRequest*)((void*)requestPointer));
            
            // if (body)
            // {
            //     response->body = reinterpret_cast<jobject>(env->NewGlobalRef(body));
            // } else {
            //     response->body = 0;
            // }

            // if (headers)
            // {
            //     int headerCount = (env->GetArrayLength(headers) / 2);
            //     int index = 0;
            //     jstring jkey;
            //     jstring jval;
            //     char const* ckey;
            //     char const * cval;
            //     for (int i=0; i<headerCount; i++)
            //     {
            //         index = i * 2;

            //         jkey = (jstring) env->GetObjectArrayElement(headers, index);
            //         jval = (jstring) env->GetObjectArrayElement(headers, (index + 1));

            //         //{TODO} again -null- is an issue
            //         if (!jkey) { 
            //             ckey = "-null-"; 
            //         } else { 
            //             ckey = env->GetStringUTFChars(jkey, NULL); 
            //         }
            //         if (!jval) { 
            //             cval = "-null-"; 
            //         } else { 
            //             cval = env->GetStringUTFChars(jval, NULL); 
            //         }

            //         response->Headers.Add(ckey,cval);
            //         response->Status = (int)responseCode;
            //         if (responseMessage!=0)
            //         {
            //             char const* rmess = env->GetStringUTFChars(responseMessage, NULL);
            //             response->ReasonPhrase = rmess;
            //             env->ReleaseStringUTFChars(responseMessage, rmess);
            //         }
                    
            //         env->ReleaseStringUTFChars(jkey, ckey);
            //         env->ReleaseStringUTFChars(jval, cval);
            //     }
            // } else {
            //     headers = 0;
            // }
            // env->DeleteLocalRef(headers);
                     
            // Xli::CTEvent* event = new Xli::CTEvent();
            // event->CTType = Xli::CTActionEvent;
            // event->Code = -1;
            // event->Payload = (void*)(new Xli::PlatformSpecific::AHttpResponseHandlerAction(((Xli::HttpResponseHandler*)((void*)requestPointer)), response));
            // GlobalWindow->EnqueueCrossThreadEvent(event);
            // //((Xli::HttpResponseHandler*)((void*)requestPointer))->OnResponse(response);
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
                {(char* const)"XliJ_HttpCallback", (char* const)"(Ljava/lang/Object;[Ljava/lang/String;ILjava/lang/String;J)V",
                 (void *)&XliJ_HttpCallback},
            };
            bool result = PlatformSpecific::AShim::RegisterNativeFunctions(nativeFuncs, 1);
            if (!result) LOGE("*** Could not register http callbacks");
            HttpInitialized = 1;
        }
    }

    //--------------------------------------------------


    HttpResponse* HttpResponse::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpResponse();
    }

    //--------------------------------------------------

    class AHttpRequest : public HttpRequest
    {
    private:
        jobject javaAsyncHandle;
        jobject javaContent;
    public:
        AHttpRequest(String url, HttpMethodType method, 
                     HttpResponseHandler* callback) 
        {
            this->Url = url;
            this->Method = method;
            this->Timeout = 0;
            this->Callback = callback;
            this->javaAsyncHandle = 0;
        }

        virtual ~AHttpRequest()
        {
        }

        virtual void Send(void* content, long byteLength)
        {
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content, byteLength);
        }

        virtual void Send(String content)
        {
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content);
        }
        
        virtual void Abort()
        {
            if (javaAsyncHandle != 0)
                PlatformSpecific::AShim::AbortAsyncConnection(javaAsyncHandle);
            javaAsyncHandle = 0;
        }

        virtual String GetContentString()
        {
            return Xli::PlatformSpecific::AShim::InputStreamToString(this->body);
        }

        virtual Stream* GetContentStream() // {TODO} this needs to be a byte array async
        {            
            return new Xli::PlatformSpecific::AStream(Xli::PlatformSpecific::AStream::READ, this->body);
        }

    };

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
