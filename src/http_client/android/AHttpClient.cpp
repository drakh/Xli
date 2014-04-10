#include <Xli/HashMap.h>
#include <XliHttpClient/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>

extern Xli::Window* GlobalWindow;

//{TODO} class/namespace alias to tidy up code

namespace Xli
{
    class AHttpRequest : public HttpRequest
    {
    private:
        String url;
        int timeout;
        virtual void EmitStateEvent();
    public:
        // the following fields should be private but due to having to set
        // them through the callbacks they aren't
        Managed< HttpStateChangedHandler > stateChangedCallback;
        Managed< HttpProgressHandler > progressCallback;
        Managed< HttpTimeoutHandler > timeoutCallback;
        Managed< HttpErrorHandler > errorCallback;
        Managed< HttpStringPulledHandler > stringPulledCallback;
        Managed< HttpArrayPulledHandler > arrayPulledCallback;
        HashMap<String,String> headers;
        HashMap<String,String> responseHeaders;
        HttpRequestState status;
        HttpMethodType method;
        int responseStatus;
        String reasonPhrase;

        jobject javaAsyncHandle;
        jobject javaContentHandle;
        String cachedContentString;
        void* cachedContentArray;
        long cachedContentArrayLength;
        bool completedSuccessfully;
        bool arrayBody;

        AHttpRequest()
        {
            this->status = HttpUnsent;
            this->url = "";
            this->method = HttpGetMethod;
            this->timeout = 0;
            this->javaAsyncHandle = 0;
            this->javaContentHandle = 0;
            this->cachedContentArray = 0;
            this->arrayBody = false;
        }

        AHttpRequest(String url, HttpMethodType method)
        {
            this->status = HttpUnsent;
            this->url = url;
            this->method = method;
            this->timeout = 0;
            this->javaAsyncHandle = 0;
            this->javaContentHandle = 0;
            this->cachedContentArray = 0;
            this->arrayBody = false;
        }

        virtual ~AHttpRequest()
        {
            PlatformSpecific::AJniHelper jni;
            if (javaAsyncHandle!=0)
            {
                jni->DeleteGlobalRef(javaAsyncHandle);
            }
            if (javaContentHandle!=0)
            {
                jni->DeleteGlobalRef(javaContentHandle);
            }
        }

        virtual HttpRequestState GetStatus() const
        {
            return this->status;
        }

        virtual void SetMethod(HttpMethodType method)
        {
            if (this->status == HttpUnsent)
            {
                this->method = method;
            } else {
                XLI_THROW("HttpRequest->SetMethod(): Not in a valid state to set the method");
            }
        }
        virtual void SetMethodFromString(String method)
        {
            if (this->status == HttpUnsent)
            {
                HttpMethodType realMethod = StringToHttpMethod(method);
                if (realMethod == HttpInvalidMethod)
                {
                    XLI_THROW("HttpRequest->SetMethodFromString(): Not a valid method type");
                } else {
                    this->method = realMethod;
                }
            } else {
                XLI_THROW("HttpRequest->SetMethodFromString(): Not in a valid state to set the method");
            }
        }
        virtual HttpMethodType GetMethod() const
        {
            return this->method;
        }

        virtual void SetUrl(String url)
        {
            if (this->status == HttpUnsent)
            {
                this->url = url;
            } else {
                XLI_THROW("HttpRequest->SetUrl(): Not in a valid state to set the url");
            }
        }
        virtual String GetUrl() const
        {
            return this->url;
        }

        virtual void SetHeader(String key, String value)
        {
            if (this->status == HttpUnsent)
            {
                this->headers.Add(key,value);
            } else {
                XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
            }
        }
        virtual void RemoveHeader(String key)
        {
            if (this->status == HttpUnsent)
            {
                this->headers.Remove(key);
            } else {
                XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
            }
        }
        virtual int HeadersBegin() const
        {
            return this->headers.Begin();
        }
        virtual int HeadersEnd() const
        {
            return this->headers.End();
        }
        virtual int HeadersNext(int n) const
        {
            return this->headers.Next(n);
        }
        virtual String GetHeaderKeyN(int n) const
        {
            return this->headers.GetKey(n);
        }
        virtual String GetHeaderValueN(int n) const
        {
            return this->headers.GetValue(n);
        }
        virtual String GetHeadersAsString() const
        {
            int i = this->HeadersBegin();
            String result = "";
            while (i != this->HeadersEnd())
            {
                result.Append(this->GetHeaderKeyN(i));
                result.Append(":");
                result.Append(this->GetHeaderValueN(i));
                result.Append("\n");
                i = this->HeadersNext(i);
            }
            return result;
        }

        virtual void SetTimeout(int timeout)
        {
            if (this->status == HttpUnsent)
            {
                this->timeout = timeout;
            } else {
                XLI_THROW("HttpRequest->SetTimeout(): Not in a valid state to set the timeout");
            }
        }
        virtual int GetTimeout() const
        {
            return this->timeout;
        }

        virtual int GetResponseStatus() const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseStatus;
            } else {
                XLI_THROW("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
            }
        }

        virtual String GetReasonPhrase() const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->reasonPhrase;
            } else {
                XLI_THROW("HttpRequest->GetReasonPhrase(): Not in a valid state to get the reason phrase");
            }
        }

        virtual int GetResponseHeaderCount() const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseHeaders.Count();
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderCount(): Not in a valid state to get the response header count");
            }
        }
        virtual int ResponseHeadersBegin() const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseHeaders.Begin();
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderBegin(): Not in a valid state to get the response header iterator");
            }
        }
        virtual int ResponseHeadersNext(int n) const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseHeaders.Next(n);
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderNext(): Not in a valid state to get the next response header");
            }
        }
        virtual int ResponseHeadersEnd() const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseHeaders.End();
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderEnd(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeaderValueN(int n) const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseHeaders.GetValue(n);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeaderKeyN(int n) const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseHeaders.GetKey(n);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeader(String key)
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseHeaders[key];
            } else {
                XLI_THROW("HttpRequest->GetResponseHeader(): Not in a valid state to get the response header");
            }
        }

        virtual void SetStateChangedCallback(HttpStateChangedHandler* callback)
        {
            if (this->status == HttpUnsent)
            {
                this->stateChangedCallback = callback;
            } else {
                XLI_THROW("HttpRequest->SetStateChangedCallback(): Not in a valid state to set the callback");
            }
        }
        virtual void SetProgressCallback(HttpProgressHandler* callback)
        {
            if (this->status == HttpUnsent)
            {
                this->progressCallback = callback;
            } else {
                XLI_THROW("HttpRequest->SetProgressCallback(): Not in a valid state to set the callback");
            }
        }
        virtual void SetTimeoutCallback(HttpTimeoutHandler* callback)
        {
            if (this->status == HttpUnsent)
            {
                this->timeoutCallback = callback;
            } else {
                XLI_THROW("HttpRequest->SetProgressCallback(): Not in a valid state to set the callback");
            }
        }
        virtual void SetErrorCallback(HttpErrorHandler* callback)
        {
            if (this->status == HttpUnsent)
            {
                this->errorCallback = callback;
            } else {
                XLI_THROW("HttpRequest->SetErrorCallback(): Not in a valid state to set the callback");
            }
        }

        virtual void SetStringPulledCallback(HttpStringPulledHandler* callback)
        {
            if (this->status == HttpUnsent)
            {
                this->stringPulledCallback = callback;
            } else {
                XLI_THROW("HttpRequest->SetStringPulledCallback(): Not in a valid state to set the callback");
            }
        }

        virtual void SetArrayPulledCallback(HttpArrayPulledHandler* callback)
        {
            if (this->status == HttpUnsent)
            {
                this->arrayPulledCallback = callback;
            } else {
                XLI_THROW("HttpRequest->SetArrayPulledCallback(): Not in a valid state to set the callback");
            }
        }

        virtual void Send(void* content, long byteLength)
        {
            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->EmitStateEvent();

            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content, byteLength);
        }

        virtual void Send(String content)
        {
            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->EmitStateEvent();
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content);
        }

        virtual void Send()
        {
            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->EmitStateEvent();
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this);
        }

        virtual void Abort()
        {
            this->status = HttpDone;
            if (this->stateChangedCallback!=0) this->EmitStateEvent();
            if (javaAsyncHandle != 0)
                PlatformSpecific::AShim::AbortAsyncConnection(javaAsyncHandle);
            javaContentHandle = 0;
            javaAsyncHandle = 0;
        }

        virtual void PullContentString()
        {
            if ((this->status==HttpHeadersReceived) && (this->javaContentHandle))
            {
                this->status = HttpLoading;
                if (this->stateChangedCallback!=0) this->EmitStateEvent();
                this->arrayBody = false;
                this-> javaAsyncHandle = PlatformSpecific::AShim::AsyncInputStreamToString(this->javaContentHandle, this);
            } else {
                XLI_THROW("HttpRequest->PullContentString(): Not in a valid state to pull the content string");
            }
        }

        virtual void PullContentArray()
        {
            if ((this->status==HttpHeadersReceived) && (this->javaContentHandle))
            {
                this->status = HttpLoading;
                if (this->stateChangedCallback!=0) this->EmitStateEvent();
                this->arrayBody = true;
                this-> javaAsyncHandle = PlatformSpecific::AShim::AsyncInputStreamToByteArray(this->javaContentHandle, this);
            } else {
                XLI_THROW("HttpRequest->PullContentArray(): Not in valid state for pulling the content array");
            }
        }
    };
    
    class AHttpTimeoutAction : public WindowAction
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
            if (this->Request->timeoutCallback!=0)
                this->Request->timeoutCallback->OnResponse(this->Request);
        }
    };
    class AHttpProgressAction : public WindowAction
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
            if (this->Request->progressCallback!=0)
                this->Request->progressCallback->OnResponse(this->Request, this->Position, this->TotalLength, this->LengthKnown);
        }
    };
    class AHttpErrorAction : public WindowAction
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
            if (this->Request->errorCallback!=0)
                this->Request->errorCallback->OnResponse(this->Request, this->ErrorCode, this->ErrorMessage);
        }
    };
    class AHttpStateAction : public WindowAction
    {
    public:
        AHttpRequest* Request;
        HttpRequestState Status;
        bool CleanJavaObjs;

        AHttpStateAction(AHttpRequest* request, HttpRequestState status, bool cleanJavaObjs = false)
        {
            this->Request = request;
            this->Status = status;
            this->CleanJavaObjs = cleanJavaObjs;
        }
        virtual ~AHttpStateAction() {}
        virtual void Execute()
        {
            if (this->CleanJavaObjs)
            {
                this->Request->javaAsyncHandle = 0;
                this->Request->javaContentHandle = 0;
            }

            if (this->Status>0) {
                this->Request->status = this->Status;
                if (this->Request->stateChangedCallback!=0)
                    this->Request->stateChangedCallback->OnResponse(this->Request, this->Status);
                if (this->Status == HttpDone)
                {
                    if (this->Request->arrayBody)
                    {
                        if (this->Request->cachedContentArray && this->Request->arrayPulledCallback)
                        {
                            this->Request->arrayPulledCallback->OnResponse(this->Request, this->Request->cachedContentArray, this->Request->cachedContentArrayLength);
                        }
                    } else if (this->Request->stringPulledCallback) {
                        this->Request->stringPulledCallback->OnResponse(this->Request, this->Request->cachedContentString);
                    }
                }
            }
        }
    };    

    extern "C"
    {
        void XliJ_CrossHttpThreadThrow(Xli::String message)
        {
            String emess = String("JAVA THROWN ERROR: ") + message;
            GlobalWindow->EnqueueCrossThreadEvent(new Xli::CTError(emess));
        }

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

                        //{TODO} NULL is an issue.. why is this being returned in the java?
                        if (!jkey) {
                            ckey = "NULL";
                        } else {
                            ckey = env->GetStringUTFChars(jkey, NULL);
                        }
                        if (!jval) {
                            cval = "NULL";
                        } else {
                            cval = env->GetStringUTFChars(jval, NULL);
                        }

                        request->responseHeaders.Add(ckey,cval);
                        request->responseStatus = (int)responseCode;
                        if (responseMessage!=0)
                        {
                            char const* rmess = env->GetStringUTFChars(responseMessage, NULL);
                            request->reasonPhrase = rmess;
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

                GlobalWindow->EnqueueCrossThreadEvent(new Xli::AHttpStateAction(request, Xli::HttpHeadersReceived));

            } else {
                XliJ_CrossHttpThreadThrow("XliJ_HttpCallback: Request pointer was not given to callback");
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
                GlobalWindow->EnqueueCrossThreadEvent(new Xli::AHttpStateAction(request, Xli::HttpDone, true));
            } else {
                LOGE("No callback pointer error");
                XliJ_CrossHttpThreadThrow("XliJ_HttpContentStringCallback: Request pointer was not given to callback");
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
                    env->DeleteLocalRef((jobject)len);
                }
                GlobalWindow->EnqueueCrossThreadEvent(new Xli::AHttpStateAction(request, Xli::HttpDone, true));
            } else {
                XliJ_CrossHttpThreadThrow("XliJ_HttpContentByteArrayCallback: Request pointer was not given to callback");
            }
        }

        void JNICALL XliJ_HttpTimeoutCallback (JNIEnv *env , jclass clazz, jlong requestPointer)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));
                GlobalWindow->EnqueueCrossThreadEvent(new Xli::AHttpTimeoutAction(request));
            } else {
                XliJ_CrossHttpThreadThrow("XliJ_HttpTimeoutCallback: Request pointer was not given to callback");
            }
        }

        void JNICALL XliJ_HttpErrorCallback (JNIEnv *env , jclass clazz, jlong requestPointer, jint errorCode, jstring errorMessage)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));

                char const* cerrorMessage = env->GetStringUTFChars(errorMessage, NULL);
                Xli::AHttpErrorAction* err =  new Xli::AHttpErrorAction(request, (int)errorCode, Xli::String(cerrorMessage));
                env->ReleaseStringUTFChars(errorMessage, cerrorMessage);

                GlobalWindow->EnqueueCrossThreadEvent(err);
            } else {
                XliJ_CrossHttpThreadThrow("XliJ_HttpErrorCallback: Request pointer was not given to callback");
            }
        }

        void JNICALL XliJ_HttpProgressCallback (JNIEnv *env , jclass clazz, jlong requestPointer, jlong position, jlong totalLength, jboolean lengthKnown)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));
                GlobalWindow->EnqueueCrossThreadEvent(new Xli::AHttpProgressAction(request, position, totalLength, lengthKnown));
            } else {
                XliJ_CrossHttpThreadThrow("XliJ_HttpProgressCallback: Request pointer was not given to callback");
            }
        }
    }


    void AHttpRequest::EmitStateEvent()
    {
        GlobalWindow->EnqueueCrossThreadEvent(new AHttpStateAction(this, this->status));
    }

    static int HttpInitialized = 0;
    static void InitAndroidHttp()
    {
        if (!HttpInitialized)
        {
            PlatformSpecific::AShim::InitDefaultCookieManager();
            static JNINativeMethod nativeFuncs[] = {
                {(char* const)"XliJ_HttpCallback", (char* const)"(Ljava/lang/Object;[Ljava/lang/String;ILjava/lang/String;J)V",
                 (void *)&XliJ_HttpCallback},
                {(char* const)"XliJ_HttpContentStringCallback", (char* const)"(Ljava/lang/String;J)V",
                 (void *)&XliJ_HttpContentStringCallback},
                {(char* const)"XliJ_HttpContentByteArrayCallback", (char* const)"([BJ)V",
                 (void *)&XliJ_HttpContentByteArrayCallback},
                {(char* const)"XliJ_HttpTimeoutCallback", (char* const)"(J)V", (void *)&XliJ_HttpTimeoutCallback},
                {(char* const)"XliJ_HttpErrorCallback", (char* const)"(JILjava/lang/String;)V",
                 (void *)&XliJ_HttpErrorCallback},
                {(char* const)"XliJ_HttpProgressCallback", (char* const)"(JJJZ)V", (void *)&XliJ_HttpProgressCallback},
            };
            bool result = PlatformSpecific::AShim::RegisterNativeFunctions(nativeFuncs, 6);
            if (result)
            {
                LOGD("XliHttp: Registered the java->c++ callbacks");
            } else {
                XLI_THROW("XliHttp: Could not register the java->c++ callbacks");
            }
            HttpInitialized = 1;
        }
    }

    HttpRequest* HttpRequest::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpRequest();
    }
};
