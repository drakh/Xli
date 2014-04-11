#include <Xli/HashMap.h>
#include <XliHttpClient/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>
#include "AHttpClient.h"

namespace Xli
{
    //--------------
    // AHttpRequest
    AHttpRequest::AHttpRequest(HttpClient* client)
    {
        this->client = client;
        this->status = HttpRequestStateUnsent;
        this->url = "";
        this->method = HttpMethodGet;
        this->timeout = 0;
        this->javaAsyncHandle = 0;
        this->javaContentHandle = 0;
        this->cachedContentArray = 0;
        this->arrayBody = false;
    }
    
    AHttpRequest::AHttpRequest(HttpClient* client, String url, HttpMethod method)            
    {
        this->client = client;
        this->status = HttpRequestStateUnsent;
        this->url = url;
        this->method = method;
        this->timeout = 0;
        this->javaAsyncHandle = 0;
        this->javaContentHandle = 0;
        this->cachedContentArray = 0;
        this->arrayBody = false;
    }

    AHttpRequest::~AHttpRequest()
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

    HttpRequestState AHttpRequest::GetStatus() const
    {
        return this->status;
    }

    void AHttpRequest::SetMethod(HttpMethod method)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->method = method;
        } else {
            XLI_THROW("HttpRequest->SetMethod(): Not in a valid state to set the method");
        }
    }
    void AHttpRequest::SetMethodFromString(String method)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            HttpMethod realMethod = StringToHttpMethod(method);
            if (realMethod == HttpMethodUnknown)
            {
                XLI_THROW("HttpRequest->SetMethodFromString(): Not a valid method type");
            } else {
                this->method = realMethod;
            }
        } else {
            XLI_THROW("HttpRequest->SetMethodFromString(): Not in a valid state to set the method");
        }
    }
    HttpMethod AHttpRequest::GetMethod() const
    {
        return this->method;
    }

    void AHttpRequest::SetUrl(String url)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->url = url;
        } else {
            XLI_THROW("HttpRequest->SetUrl(): Not in a valid state to set the url");
        }
    }
    String AHttpRequest::GetUrl() const
    {
        return this->url;
    }

    void AHttpRequest::SetHeader(String key, String value)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->headers.Add(key,value);
        } else {
            XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
        }
    }
    void AHttpRequest::RemoveHeader(String key)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->headers.Remove(key);
        } else {
            XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
        }
    }
    int AHttpRequest::HeadersBegin() const
    {
        return this->headers.Begin();
    }
    int AHttpRequest::HeadersEnd() const
    {
        return this->headers.End();
    }
    int AHttpRequest::HeadersNext(int n) const
    {
        return this->headers.Next(n);
    }
    String AHttpRequest::GetHeaderKeyN(int n) const
    {
        return this->headers.GetKey(n);
    }
    String AHttpRequest::GetHeaderValueN(int n) const
    {
        return this->headers.GetValue(n);
    }
    String AHttpRequest::GetHeadersAsString() const
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

    void AHttpRequest::SetTimeout(int timeout)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->timeout = timeout;
        } else {
            XLI_THROW("HttpRequest->SetTimeout(): Not in a valid state to set the timeout");
        }
    }
    int AHttpRequest::GetTimeout() const
    {
        return this->timeout;
    }

    int AHttpRequest::GetResponseStatus() const
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->responseStatus;
        } else {
            XLI_THROW("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
        }
    }

    String AHttpRequest::GetReasonPhrase() const
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->reasonPhrase;
        } else {
            XLI_THROW("HttpRequest->GetReasonPhrase(): Not in a valid state to get the reason phrase");
        }
    }

    int AHttpRequest::GetResponseHeaderCount() const
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->responseHeaders.Count();
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderCount(): Not in a valid state to get the response header count");
        }
    }
    int AHttpRequest::ResponseHeadersBegin() const
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->responseHeaders.Begin();
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderBegin(): Not in a valid state to get the response header iterator");
        }
    }
    int AHttpRequest::ResponseHeadersNext(int n) const
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->responseHeaders.Next(n);
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderNext(): Not in a valid state to get the next response header");
        }
    }
    int AHttpRequest::ResponseHeadersEnd() const
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->responseHeaders.End();
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderEnd(): Not in a valid state to get the response header");
        }
    }
    String AHttpRequest::GetResponseHeaderValueN(int n) const
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->responseHeaders.GetValue(n);
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
        }
    }
    String AHttpRequest::GetResponseHeaderKeyN(int n) const
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->responseHeaders.GetKey(n);
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
        }
    }
    String AHttpRequest::GetResponseHeader(String key)
    {
        if (this->status == HttpRequestStateHeadersReceived)
        {
            return this->responseHeaders[key];
        } else {
            XLI_THROW("HttpRequest->GetResponseHeader(): Not in a valid state to get the response header");
        }
    }

    void AHttpRequest::SetStateChangedCallback(HttpStateChangedHandler* callback)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->stateChangedCallback = callback;
        } else {
            XLI_THROW("HttpRequest->SetStateChangedCallback(): Not in a valid state to set the callback");
        }
    }
    void AHttpRequest::SetProgressCallback(HttpProgressHandler* callback)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->progressCallback = callback;
        } else {
            XLI_THROW("HttpRequest->SetProgressCallback(): Not in a valid state to set the callback");
        }
    }
    void AHttpRequest::SetTimeoutCallback(HttpTimeoutHandler* callback)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->timeoutCallback = callback;
        } else {
            XLI_THROW("HttpRequest->SetProgressCallback(): Not in a valid state to set the callback");
        }
    }
    void AHttpRequest::SetErrorCallback(HttpErrorHandler* callback)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->errorCallback = callback;
        } else {
            XLI_THROW("HttpRequest->SetErrorCallback(): Not in a valid state to set the callback");
        }
    }

    void AHttpRequest::SetStringPulledCallback(HttpStringPulledHandler* callback)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->stringPulledCallback = callback;
        } else {
            XLI_THROW("HttpRequest->SetStringPulledCallback(): Not in a valid state to set the callback");
        }
    }

    void AHttpRequest::SetArrayPulledCallback(HttpArrayPulledHandler* callback)
    {
        if (this->status == HttpRequestStateUnsent)
        {
            this->arrayPulledCallback = callback;
        } else {
            XLI_THROW("HttpRequest->SetArrayPulledCallback(): Not in a valid state to set the callback");
        }
    }

    void AHttpRequest::SendASync(void* content, long byteLength)
    {
        this->status = HttpRequestStateSent;
        if (this->stateChangedCallback!=0) this->client->EnqueueAction(new AHttpStateAction(this, this->status));

        javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content, byteLength);
    }

    void AHttpRequest::SendASync(String content)
    {
        this->status = HttpRequestStateSent;
        if (this->stateChangedCallback!=0) this->client->EnqueueAction(new AHttpStateAction(this, this->status));
        javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content);
    }

    void AHttpRequest::SendASync()
    {
        this->status = HttpRequestStateSent;
        if (this->stateChangedCallback!=0) this->client->EnqueueAction(new AHttpStateAction(this, this->status));
        javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this);
    }

    void AHttpRequest::Abort()
    {
        this->status = HttpRequestStateDone;
        if (this->stateChangedCallback!=0) this->client->EnqueueAction(new AHttpStateAction(this, this->status));
        if (javaAsyncHandle != 0)
            PlatformSpecific::AShim::AbortAsyncConnection(javaAsyncHandle);
        javaContentHandle = 0;
        javaAsyncHandle = 0;
    }

    void AHttpRequest::PullContentString()
    {
        if ((this->status==HttpRequestStateHeadersReceived) && (this->javaContentHandle))
        {
            this->status = HttpRequestStateLoading;
            if (this->stateChangedCallback!=0) this->client->EnqueueAction(new AHttpStateAction(this, this->status));
            this->arrayBody = false;
            this-> javaAsyncHandle = PlatformSpecific::AShim::AsyncInputStreamToString(this->javaContentHandle, this);
        } else {
            XLI_THROW("HttpRequest->PullContentString(): Not in a valid state to pull the content string");
        }
    }

    void AHttpRequest::PullContentArray()
    {
        if ((this->status==HttpRequestStateHeadersReceived) && (this->javaContentHandle))
        {
            this->status = HttpRequestStateLoading;
            if (this->stateChangedCallback!=0) this->client->EnqueueAction(new AHttpStateAction(this, this->status));
            this->arrayBody = true;
            this-> javaAsyncHandle = PlatformSpecific::AShim::AsyncInputStreamToByteArray(this->javaContentHandle, this);
        } else {
            XLI_THROW("HttpRequest->PullContentArray(): Not in valid state for pulling the content array");
        }
    }

    //----------------------------------------
    // Actions for cross thread communication
    AHttpTimeoutAction::AHttpTimeoutAction(AHttpRequest* request)
    {
        this->Request = request;
    }
    void AHttpTimeoutAction::Execute()
    {
        if (this->Request->timeoutCallback!=0)
            this->Request->timeoutCallback->OnResponse(this->Request);
    }

    AHttpProgressAction::AHttpProgressAction(AHttpRequest* request, long position, long totalLength, bool lengthKnown)
    {
        this->Request = request;
        this->Position = position;
        this->TotalLength = totalLength;
        this->LengthKnown = lengthKnown;
    }
    void AHttpProgressAction::Execute()
    {
        if (this->Request->progressCallback!=0)
            this->Request->progressCallback->OnResponse(this->Request, this->Position, this->TotalLength, this->LengthKnown);
    }


    AHttpErrorAction::AHttpErrorAction(AHttpRequest* request, int errorCode, String errorMessage)
    {
        this->Request = request;
        this->ErrorCode = errorCode;
        this->ErrorMessage = errorMessage;
    }
    void AHttpErrorAction::Execute()
    {
        if (this->Request->errorCallback!=0)
            this->Request->errorCallback->OnResponse(this->Request, this->ErrorCode, this->ErrorMessage);
    }

    AHttpStateAction::AHttpStateAction(AHttpRequest* request, HttpRequestState status, bool cleanJavaObjs)
    {
        this->Request = request;
        this->Status = status;
        this->CleanJavaObjs = cleanJavaObjs;
    }
    void AHttpStateAction::Execute()
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
            if (this->Status == HttpRequestStateDone)
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


    //-----------------------------------------
    // Entry points fors calls from java to c++ 
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

                        if (!jkey) {
                            ckey = "";
                        } else {
                            ckey = env->GetStringUTFChars(jkey, NULL);
                        }
                        if (!jval) {
                            cval = "";
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

                request->client->EnqueueAction(new Xli::AHttpStateAction(request, Xli::HttpRequestStateHeadersReceived));

            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
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
                request->client->EnqueueAction(new Xli::AHttpStateAction(request, Xli::HttpRequestStateDone, true));
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
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
                request->client->EnqueueAction(new Xli::AHttpStateAction(request, Xli::HttpRequestStateDone, true));
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }

        void JNICALL XliJ_HttpTimeoutCallback (JNIEnv *env , jclass clazz, jlong requestPointer)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));
                request->client->EnqueueAction(new Xli::AHttpTimeoutAction(request));
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
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

                request->client->EnqueueAction(err);
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }

        void JNICALL XliJ_HttpProgressCallback (JNIEnv *env , jclass clazz, jlong requestPointer, jlong position, jlong totalLength, jboolean lengthKnown)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));
                request->client->EnqueueAction(new Xli::AHttpProgressAction(request, position, totalLength, lengthKnown));
            } else {
                LOGE("CRITICAL HTTP ERROR: No callback pointer error");
            }
        }
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

    class AHttpClient : public HttpClient
    {
    private:
        MutexQueue<HttpAction*> actionQueue;
    public:
        virtual AHttpRequest* CreateRequest()
        {
            return new AHttpRequest(this);
        }

        virtual void Update()
        {
            while ((actionQueue.Count() > 0))
            {
                HttpAction* action = actionQueue.Dequeue();
                action->Execute();
                delete action;
            }
        }

        virtual void EnqueueAction(HttpAction* action)
        {
            actionQueue.Enqueue(action);
        }
    };

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new AHttpClient();
    }
};
