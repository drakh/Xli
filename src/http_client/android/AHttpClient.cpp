#include <Xli/HashMap.h>
#include <XliHttpClient/HttpClient.h>
#include <Xli/PlatformSpecific/Android.h>
#include "AHttpClient.h"
#include "../../core/android/AJniHelper.h"

extern Xli::Window* GlobalWindow;

namespace Xli
{
    //---- AHttpRequest ----//
    
    AHttpRequest::AHttpRequest(HttpClient* client, String url, HttpMethod method)            
    {
        this->client = client;
        this->state = HttpRequestStateUnsent;
        this->url = url;
        this->method = method;
        this->timeout = 0;
        this->javaAsyncHandle = 0;
        this->javaContentHandle = 0;
        this->cachedContentArray = 0;
    }

    AHttpRequest::~AHttpRequest()
    {
        CleanHandles();
    }

    void AHttpRequest::CleanHandles()
    {
        if (javaAsyncHandle!=0) jni->DeleteGlobalRef(javaAsyncHandle);
        if (javaContentHandle!=0) jni->DeleteGlobalRef(javaContentHandle);
        javaAsyncHandle = 0;
        javaContentHandle = 0;
    }

    HttpMethod AHttpRequest::GetMethod() const { return method; }
    String AHttpRequest::GetUrl() const { return url; }
    HttpRequestState AHttpRequest::GetState() const { return state; }
    int AHttpRequest::GetTimeout() const { return timeout; }

    void AHttpRequest::SetTimeout(int timeout)
    {
        if (state == HttpRequestStateUnsent)
        {
            this->timeout = timeout;
        } else {
            XLI_THROW("HttpRequest->SetTimeout(): Not in a valid state to set the timeout");
        }
    }    

    void AHttpRequest::SetHeader(String key, String value)
    {
        if (state == HttpRequestStateUnsent)
        {
            headers.Add(key,value);
        } else {
            XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
        }
    }
    void AHttpRequest::RemoveHeader(String key)
    {
        if (state == HttpRequestStateUnsent)
        {
            headers.Remove(key);
        } else {
            XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
        }
    }

    int AHttpRequest::HeadersBegin() const { return headers.Begin(); }
    int AHttpRequest::HeadersEnd() const { return headers.End(); }
    int AHttpRequest::HeadersNext(int n) const { return headers.Next(n); }
    String AHttpRequest::GetHeaderKey(int n) const { return headers.GetKey(n); }
    String AHttpRequest::GetHeaderValue(int n) const { return headers.GetValue(n); }

    void AHttpRequest::SendASync(void* content, long byteLength)
    {
        if (state == HttpRequestStateUnsent) 
        {
            state = HttpRequestStateSent;
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content, byteLength);
        } else {
            XLI_THROW("HttpRequest->SendASync(): Not in a valid state to send");
        }
    }

    void AHttpRequest::SendASync(String content)
    {
        if (state == HttpRequestStateUnsent) 
        {
            state = HttpRequestStateSent;
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content);
        } else {
            XLI_THROW("HttpRequest->SendASync(): Not in a valid state to send");
        }
    }

    void AHttpRequest::SendASync()
    {
        if (state == HttpRequestStateUnsent) 
        {
            state = HttpRequestStateSent;
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this);
        } else {
            XLI_THROW("HttpRequest->SendASync(): Not in a valid state to send");
        }
    }

    void AHttpRequest::Abort()
    {
        if ((int)state > (int)HttpRequestStateUnsent) 
        {
            state = HttpRequestStateDone;
            if (javaAsyncHandle != 0) PlatformSpecific::AShim::AbortAsyncConnection(javaAsyncHandle);
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestAborted(this);
            CleanHandles();
        } else {
            XLI_THROW("HttpRequest->SendASync(): Request has not beem sent, cannot abort");
        }
    }
    void AHttpRequest::StartDownload()
    {
        if ((this->state==HttpRequestStateHeadersReceived) && (this->javaContentHandle))
        {
            this-> javaAsyncHandle = PlatformSpecific::AShim::AsyncInputStreamToByteArray(this->javaContentHandle, this);
            this->state = HttpRequestStateLoading;           
            HttpEventHandler* eh = Request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);            
        } else {
            XLI_THROW("HttpRequest->PullContentArray(): Not in valid state for pulling the content array");
        }
    }

    int AHttpRequest::GetResponseHeaderCount() const
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return responseHeaders.Count();
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderCount(): Not in a valid state to get the response header count");
        }
    }
    int AHttpRequest::ResponseHeadersBegin() const
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return responseHeaders.Begin();
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderBegin(): Not in a valid state to get the response header iterator");
        }
    }
    int AHttpRequest::ResponseHeadersNext(int n) const
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return responseHeaders.Next(n);
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderNext(): Not in a valid state to get the next response header");
        }
    }
    int AHttpRequest::ResponseHeadersEnd() const
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return responseHeaders.End();
        } else {
            XLI_THROW("HttpRequest->ResponseHeaderEnd(): Not in a valid state to get the response header");
        }
    }
    String AHttpRequest::GetResponseHeaderValue(int n) const
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return responseHeaders.GetValue(n);
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
        }
    }
    String AHttpRequest::GetResponseHeaderKey(int n) const
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return responseHeaders.GetKey(n);
        } else {
            XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
        }
    }

    bool AHttpRequest::TryGetResponseHeader(const String& key, String& result) const
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return responseHeaders->TryGetValue(key, result);
        } else {
            XLI_THROW("HttpRequest->GetResponseHeader(): Not in a valid state to get the response header");
        }
    }

    int AHttpRequest::GetResponseStatus() const
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return responseStatus;
        } else {
            XLI_THROW("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
        }
    }
    
    DataAccessor* AHttpRequest::GetResponseBody()
    {
        if (state == HttpRequestStateHeadersReceived)
        {
            return (DataAccessor*)0;
        } else {
            XLI_THROW("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
        }
    }


    //---- Entry points fors calls from java to c++ ----//
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
                    //{TODO} check
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
                Request->StartDownload();
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
                    request->responseBody = Buffer::Create((int)len);                    
                    env->GetByteArrayRegion(content, 0, len, (jbyte*)request->responseBody->DataPtr());

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

        void JNICALL XliJ_HttpProgressCallback (JNIEnv *env , jclass clazz, jlong requestPointer, jlong position, jlong totalLength, jboolean lengthKnown, jint direction)
        {
            if (requestPointer)
            {
                Xli::AHttpRequest* request = ((Xli::AHttpRequest*)((void*)requestPointer));
                request->client->EnqueueAction(new Xli::AHttpProgressAction(request, position, totalLength, lengthKnown,
                                                                            (HttpTransferDirection)(int)direction)));
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
                {(char* const)"XliJ_HttpContentByteArrayCallback", (char* const)"([BJ)V",
                 (void *)&XliJ_HttpContentByteArrayCallback},
                {(char* const)"XliJ_HttpTimeoutCallback", (char* const)"(J)V", (void *)&XliJ_HttpTimeoutCallback},
                {(char* const)"XliJ_HttpErrorCallback", (char* const)"(JILjava/lang/String;)V",
                 (void *)&XliJ_HttpErrorCallback},
                {(char* const)"XliJ_HttpProgressCallback", (char* const)"(JJJZI)V", (void *)&XliJ_HttpProgressCallback},
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
