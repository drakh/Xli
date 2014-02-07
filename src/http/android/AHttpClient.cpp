#include <Xli/HashMap.h>
#include <XliHttp/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>

extern Xli::Window* GlobalWindow;

namespace Xli
{

    class AHttpRequest : public HttpRequest
    {
    public:
        jobject javaAsyncHandle;
        jobject javaContentHandle;
        String cachedContentString; //Error when make managed: 'class Xli::String' has no member named 'Release'
        void* cachedContentArray;
        long cachedContentArrayLength;

        AHttpRequest() 
        {
            this->Status = HttpUnsent;
            this->Url = "";
            this->Method = HttpGetMethod;
            this->Timeout = 0;
            this->javaAsyncHandle = 0;
            this->javaContentHandle = 0;
            this->cachedContentArray = 0;
        }

        AHttpRequest(String url, HttpMethodType method) 
        {
            this->Status = HttpUnsent;
            this->Url = url;
            this->Method = method;
            this->Timeout = 0;
            this->javaAsyncHandle = 0;
            this->javaContentHandle = 0;
            this->cachedContentArray = 0;
        }

        virtual ~AHttpRequest()
        {
            //{TODO} Oooo ME ME!
        }

        virtual void Send(void* content, long byteLength)
        {
            this->Status = HttpSent;
            if (this->StateChangedCallback!=0) this->StateChangedCallback->OnResponse(this, this->Status);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content, byteLength);
        }

        virtual void Send(String content)
        {
            this->Status = HttpSent;
            if (this->StateChangedCallback!=0) this->StateChangedCallback->OnResponse(this, this->Status);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content);
        }

        virtual void Send()
        {
            this->Status = HttpSent;
            if (this->StateChangedCallback!=0) this->StateChangedCallback->OnResponse(this, this->Status);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this);
        }
        
        virtual void Abort()
        {
            this->Status = HttpDone; //{TODO} how does statechanged know if it was successful?
            if (this->StateChangedCallback!=0) this->StateChangedCallback->OnResponse(this, this->Status);
            if (javaAsyncHandle != 0)
                PlatformSpecific::AShim::AbortAsyncConnection(javaAsyncHandle);
            javaContentHandle = 0;
            javaAsyncHandle = 0;
        }

        virtual void PullContentString()
        {
            if ((this->Status==HttpHeadersReceived) && (this->javaContentHandle))
            {
                this->Status = HttpLoading;
                if (this->StateChangedCallback!=0) this->StateChangedCallback->OnResponse(this, this->Status);
                this-> javaAsyncHandle = Xli::PlatformSpecific::AShim::AsyncInputStreamToString(this->javaContentHandle, this);
            } else {
                LOGE("Cant pull content string"); //{TODO} proper error here
            }
        }
        virtual String GetContentString()
        {
            if (this->Status == HttpDone)
                return this->cachedContentString;
            LOGE("Cant get content string");
            return ""; // {TODO} throw error?
        }        

        virtual void PullContentArray() // {TODO} this needs to be a byte array async
        {    
            if ((this->Status==HttpHeadersReceived) && (this->javaContentHandle))
            {
                this->Status = HttpLoading;
                if (this->StateChangedCallback!=0) this->StateChangedCallback->OnResponse(this, this->Status);
                this-> javaAsyncHandle = Xli::PlatformSpecific::AShim::AsyncInputStreamToByteArray(this->javaContentHandle, this);
            } else {
                LOGE("Cant pull content stream"); //{TODO} proper error here
            }
        }
        virtual void* GetContentArray() // {TODO} this needs to be a byte array async
        {            
            if (this->Status == HttpDone)
                return this->cachedContentArray;
            LOGE("Cant get content array");
        }
        virtual long GetContentArrayLength()
        {
            if ((this->Status == HttpDone) && (this->cachedContentArray!=0))
                return this->cachedContentArrayLength;
            LOGE("Cant get content array length");
            return -1; // {TODO} throw error?
        }
    };

    namespace PlatformSpecific
    {
        class AHttpTimeoutAction : public AWindowAction
        {    
        public:
            AHttpRequest* Request;

            AHttpTimeoutAction(AHttpRequest* request)
            {
                this->Request = request;
            }
            virtual ~AHttpTimeoutAction() {}
            virtual void Execute()
            {                       
                if (this->Request->TimeoutCallback!=0)
                    this->Request->TimeoutCallback->OnResponse(this->Request);                
            }
        };
        class AHttpProgressAction : public AWindowAction
        {    
        public:
            AHttpRequest* Request;
            long Position;
            long TotalLength;
            bool LengthKnown;

            AHttpProgressAction(AHttpRequest* request, long position, long totalLength, bool lengthKnown)
            {
                this->Request = request;
                this->Position = position;
                this->TotalLength = totalLength;
                this->LengthKnown = lengthKnown;
            }
            virtual ~AHttpProgressAction() {}
            virtual void Execute()
            {                       
                if (this->Request->ProgressCallback!=0)
                    this->Request->ProgressCallback->OnResponse(this->Request, this->Position, this->TotalLength, this->LengthKnown);
            }
        };
        class AHttpErrorAction : public AWindowAction
        {    
        public:
            AHttpRequest* Request;
            int ErrorCode;
            String ErrorMessage;

            AHttpErrorAction(AHttpRequest* request, int errorCode, String errorMessage)
            {
                this->Request = request;
                this->ErrorCode = errorCode;
                this->ErrorMessage = errorMessage;
            }
            virtual ~AHttpErrorAction() {}
            virtual void Execute()
            {                       
                if (this->Request->ErrorCallback!=0)
                    this->Request->ErrorCallback->OnResponse(this->Request, this->ErrorCode, this->ErrorMessage);
            }
        };
        class AHttpResponseHandlerAction : public AWindowAction
        {    
        public:
            AHttpRequest* Request;
            HttpRequestState Status;
            bool CleanJavaObjs;

            AHttpResponseHandlerAction(AHttpRequest* request, HttpRequestState status, bool cleanJavaObjs = false)
            {
                this->Request = request;
                this->Status = status;
                this->CleanJavaObjs = cleanJavaObjs;
            }
            virtual ~AHttpResponseHandlerAction() {}
            virtual void Execute()
            {    
                if (this->CleanJavaObjs)
                {
                    this->Request->javaAsyncHandle = 0;
                    this->Request->javaContentHandle = 0;
                }
                    
                if (this->Status>0) {
                    this->Request->Status = this->Status;
                    if (this->Request->StateChangedCallback!=0)
                        this->Request->StateChangedCallback->OnResponse(this->Request, this->Status);
                }
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
            Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));
            if (body)
            {
                request->javaContentHandle = reinterpret_cast<jobject>(env->NewGlobalRef(body));
            } else {
                request->javaContentHandle = 0;
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

                    request->Headers.Add(ckey,cval);
                    request->ResponseStatus = (int)responseCode;
                    if (responseMessage!=0)
                    {
                        char const* rmess = env->GetStringUTFChars(responseMessage, NULL);
                        request->ReasonPhrase = rmess;
                        env->ReleaseStringUTFChars(responseMessage, rmess);
                    }
                    
                    env->ReleaseStringUTFChars(jkey, ckey);
                    env->ReleaseStringUTFChars(jval, cval);
                }
            } else {
                headers = 0;
            }
            env->DeleteLocalRef(headers);
            request->javaAsyncHandle = 0;

            Xli::CTEvent* event = new Xli::CTEvent();
            event->CTType = Xli::CTActionEvent;
            event->Code = -1;
            event->Payload = (void*)(new Xli::PlatformSpecific::AHttpResponseHandlerAction(request, Xli::HttpHeadersReceived));
            GlobalWindow->EnqueueCrossThreadEvent(event);

        } else {
            LOGE("No callback pointer error");
        }
    }
    void JNICALL XliJ_HttpContentStringCallback (JNIEnv *env , jclass clazz, jstring content, jlong requestPointer)
    {
        if (requestPointer)
        {
            Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));
            char const* ccontent = env->GetStringUTFChars(content, NULL);
            request->cachedContentString = ccontent;
            env->ReleaseStringUTFChars(content, ccontent);
            Xli::CTEvent* event = new Xli::CTEvent();
            event->CTType = Xli::CTActionEvent;
            event->Code = -1;
            event->Payload = (void*)(new Xli::PlatformSpecific::AHttpResponseHandlerAction(request, Xli::HttpDone, true));
            GlobalWindow->EnqueueCrossThreadEvent(event);
        } else {
            LOGE("No callback pointer error");
        }
    }
    void JNICALL XliJ_HttpContentByteArrayCallback (JNIEnv *env , jclass clazz, jbyteArray content, jlong requestPointer)
    {
        if (requestPointer)
        {
            Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

            if (content != 0) {
                jsize len = env->GetArrayLength(content);
                if (request->cachedContentArray!=0) free(request->cachedContentArray); // Should never happen though
                request->cachedContentArray = malloc((long)len);
                env->GetByteArrayRegion(content, 0, len, (jbyte*)request->cachedContentArray);
                request->cachedContentArrayLength = (long)len;
                env->DeleteLocalRef(content);
                //env->DeleteLocalRef((jobject)len); {TODO} test this, is in neccessary
            }

            Xli::CTEvent* event = new Xli::CTEvent();
            event->CTType = Xli::CTActionEvent;
            event->Code = -1;
            event->Payload = (void*)(new Xli::PlatformSpecific::AHttpResponseHandlerAction(request, Xli::HttpDone, true));
            GlobalWindow->EnqueueCrossThreadEvent(event);
        } else {
            LOGE("No callback pointer error");
        }
    }
    
    void JNICALL XliJ_HttpTimeoutCallback (JNIEnv *env , jclass clazz, jlong requestPointer)
    {
        if (requestPointer)
        {
            Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

            Xli::CTEvent* event = new Xli::CTEvent();
            event->CTType = Xli::CTActionEvent;
            event->Code = -1;
            event->Payload = (void*)(new Xli::PlatformSpecific::AHttpTimeoutAction(request));
            GlobalWindow->EnqueueCrossThreadEvent(event);
        } else {
            LOGE("No callback pointer error");
        }
    }

    void JNICALL XliJ_HttpErrorCallback (JNIEnv *env , jclass clazz, jlong requestPointer, jint errorCode, jstring errorMessage)
    {
        if (requestPointer)
        {
            Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

            Xli::CTEvent* event = new Xli::CTEvent();
            event->CTType = Xli::CTActionEvent;
            event->Code = -1;

            char const* cerrorMessage = env->GetStringUTFChars(errorMessage, NULL);
            event->Payload = (void*)(new Xli::PlatformSpecific::AHttpErrorAction(request, (int)errorCode, Xli::String(cerrorMessage)));
            env->ReleaseStringUTFChars(errorMessage, cerrorMessage);

            GlobalWindow->EnqueueCrossThreadEvent(event);
        } else {
            LOGE("No callback pointer error");
        }
    }

    void JNICALL XliJ_HttpProgressCallback (JNIEnv *env , jclass clazz, jlong requestPointer, jlong position, jlong totalLength, jboolean lengthKnown)
    {
        if (requestPointer)
        {
            Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

            Xli::CTEvent* event = new Xli::CTEvent();
            event->CTType = Xli::CTActionEvent;
            event->Code = -1;
            event->Payload = (void*)(new Xli::PlatformSpecific::AHttpProgressAction(request, position, totalLength, lengthKnown));
            GlobalWindow->EnqueueCrossThreadEvent(event);
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
                {(char* const)"XliJ_HttpCallback", (char* const)"(Ljava/lang/Object;[Ljava/lang/String;ILjava/lang/String;J)V", (void *)&XliJ_HttpCallback},
                {(char* const)"XliJ_HttpContentStringCallback", (char* const)"(Ljava/lang/String;J)V", (void *)&XliJ_HttpContentStringCallback},
                {(char* const)"XliJ_HttpContentByteArrayCallback", (char* const)"([BJ)V", (void *)&XliJ_HttpContentByteArrayCallback}, 
                {(char* const)"XliJ_HttpTimeoutCallback", (char* const)"(J)V", (void *)&XliJ_HttpTimeoutCallback}, 
                {(char* const)"XliJ_HttpErrorCallback", (char* const)"(JILjava/lang/String;)V", (void *)&XliJ_HttpErrorCallback}, 
                {(char* const)"XliJ_HttpProgressCallback", (char* const)"(JJJZ)V", (void *)&XliJ_HttpProgressCallback}, 
            };
            bool result = PlatformSpecific::AShim::RegisterNativeFunctions(nativeFuncs, 6);
            if (!result) LOGE("*** Could not register http callbacks");
            HttpInitialized = 1;
        }
    }

    class AHttpClient : public HttpClient
    {
    public:
        AHttpClient() {}
        virtual ~AHttpClient() {}
        virtual HttpRequest* NewRequest()
        {
            return new AHttpRequest();
        }
    };

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpClient();
    }
};
