#include <XliHttp/HttpClient.h>
#include <CurlHttpClient.h>
#include <Xli/HashMap.h>
#include <curl/curl.h>
#include <pthread.h>

namespace Xli
{
    class CurlHook
    {
    public:
        static int HttpInitialized = 0;
        CURLcode CurlGlobalCode;

        static void Init()
        {
            if (!HttpInitialized)
            {
                CurlGlobalCode = curl_global_init(CURL_GLOBAL_ALL);
                HttpInitialized = 1;
            }
        }
        static void Close()
        {
            curl_global_cleanup();
        }
    };

    class CurlHttpRequest : public HttpRequest
    {
    private:
        Managed< HttpStateChangedHandler > stateChangedCallback;
        Managed< HttpProgressHandler > progressCallback;
        Managed< HttpTimeoutHandler > timeoutCallback;
        Managed< HttpErrorHandler > errorCallback;
        Managed< HttpStringPulledHandler > stringPulledCallback;
        Managed< HttpArrayPulledHandler > arrayPulledCallback;
        HttpMethodType method;
        CURL* curlSession;
        int timeout;
        String url;
        Array<UInt8> recievedData;
        String recievedString;
        bool dataReady;
        bool reading;
    public:
        CurlHttpRequest()
        {
            this->status = HttpUnsent;
            this->url = "";
            this->method = HttpGetMethod;
            this->timeout = 0;
            this->recievedData = malloc(1);
            this->dataReady = false;
            this->reading = false;
        }
        CurlHttpRequest(String url, HttpMethodType method)
        {
            this->status = HttpUnsent;
            this->url = url;
            this->method = method;
            this->timeout = 0;
            this->recievedData = malloc(1);
            this->dataReady = false;
            this->reading = false;
        }

        virtual ~CurlHttpRequest()
        {
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
                XLI_THROW("HttpRequest->SetArrayPulledCallback(): Not in a valid state to set the virtual");
            }
        }

        void callback Send(void* content, long byteLength)
        {
            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->emitStateEvent();

            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content, byteLength);
        }

        virtual void Send(String content)
        {
            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->emitStateEvent();
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this, content);
        }

        virtual void Send()
        {
            CURL* session = curl_easy_init();
            if (!session)
                XLI_THROW("CURL ERROR: Failed to create handle");

            CURLcode result = CURLE_OK;
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_URL, url.DataPtr());
            if (result == CURLE_OK) result = curl_easy_setopt(session, methodToCurlCode(self->method), 1);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_CONNECTTIMEOUT, self->timeout);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_AUTOREFERER, 1);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, onDataRecieved);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_WRITEDATA, (void*)this);

            //progress callback
            if (this->progressCallback!=0 &&)
            {
                result = curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
                result = curl_easy_setopt(session, CURLOPT_XFERINFODATA, (void*)this);
                result = curl_easy_setopt(session, CURLOPT_XFERINFOFUNCTION, onProgress);
            }

            //Method specific options
            switch(method)
            {
            case HttpGetMethod:
                break;
            case HttpPostMethod:
                break;
            case HttpOptionsMethod:
                break;
            case HttpHeadMethod:
                break;
            case HttpPutMethod:
                // CURLOPT_READDATA and CURLOPT_INFILESIZE or CURLOPT_INFILESIZE_LARGE
                break;
            case HttpDeleteMethod:
                break;
            case HttpTraceMethod:
                break;
            };
            //CURLOPT_NOBODY

            if (result == CURLE_OK) result = curl_easy_perform(session); //{TODO} and if it isnt?

            this->perform(CURL* session);
        }

    private:
        virtual perform(CURL* session)
        {
            //signal state changed
            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->emitStateEvent();
            
            //start the curl request. curl_easy_perform will block
            this->session = session;
            CURLcode result = curl_easy_perform(session);
            if (result != 0)
                dispatchError(result);
        }
        virtual void dispatchError(CURLcode error)
        {
            switch(method)
            {
            case CURLE_OPERATION_TIMEOUT : onTimeout(this); break;
            default: XLI_THROW("XLI CURL: UNHANDLED CURL ERROR."); break;
            };
        }

        virtual CURLcode methodToCurlCode(HttpMethodType method)
        {
            switch(method)
            {
            case HttpGetMethod: return CURLOPT_GET;
            case HttpPostMethod: return CURLOPT_POST;
            case HttpOptionsMethod: return CURLOPT_OPTIONS;
            case HttpHeadMethod: return CURLOPT_HEAD;
            case HttpPutMethod: return CURLOPT_UPLOAD;
            case HttpDeleteMethod: return CURLOPT_DELETE;
            case HttpTraceMethod: return CURLOPT_TRACE;
            default: XLI_THROW("MethodToCurlCode: Invalid Method Code");
            };
        }
        void CurlHttpRequest::emitStateEvent()
        {
            GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpStateAction(this, this->status));
        }
        static size_t onDataRecieved(char* ptr, size_t size, size_t nmemb, void* userdata);
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;            

            request->reading = true  //{TODO} this is a temporary hack

            if (request->reading == true)
            {
                request->dataReady = true;
                if (request->contentAsString) {
                    onStringDataRecieved(request, ptr, (size * nmemb));
                } else {
                    onByteDataRecieved(request, ptr, (size * nmemb));
                }
            } else {
                request->dataReady = true;
            }
        }
        static size_t onStringDataRecieved(CurlHttpRequest* request, char* ptr, size_t bytesRecieved)
        {
            request->recievedString.Append((char*)ptr, bytesRecieved);
        }
        static size_t onByteDataRecieved(CurlHttpRequest* request, char* ptr, size_t bytesRecieved)
        {
            request->recievedData.AddRange((UInt8*)ptr, bytesRecieved);            
        }
        static void onStateChanged()
        {
        }
        static int onProgress(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;
            UInt64 total = (UInt64)dltotal;
            UInt64 pos = (UInt64)dlnow;
        }
        static void onTimeout(CurlHttpRequest* request)
        {
            
        }
        static void onError()
        {
        }
        static void onStringPulled()
        {
        }
        static void onArrayPulled()
        {
        }
    };

    //------------------------------------------------------------

    CurlHttpStateAction::CHttpStateAction(CurlHttpRequest* request, HttpRequestState status)
    {
        this->Request = request;
        this->Status = status;
    }
    void CurlHttpStateAction::Execute()
    {
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
                        //{TODO} Fix me
                        //this->Request->arrayPulledCallback->OnResponse(this->Request, this->Request->cachedContentArray, this->Request->cachedContentArrayLength);
                    }
                } else if (this->Request->stringPulledCallback) {
                    this->Request->stringPulledCallback->OnResponse(this->Request, this->Request->recievedString);
                }
            }
        }
    }

    CurlHttpTimeoutAction::CurlHttpTimeoutAction(CurlHttpRequest* request)
    {
        this->Request = request;
    }
    void CurlHttpTimeoutAction::Execute()
    {
        if (this->Request->timeoutCallback!=0)
            this->Request->timeoutCallback->OnResponse(this->Request);
    }

    CurlHttpProgressAction::CurlHttpProgressAction(CurlHttpRequest* request, long position, long totalLength, bool lengthKnown)
    {
        this->Request = request;
        this->Position = position;
        this->TotalLength = totalLength;
        this->LengthKnown = lengthKnown;
    }
    void CurlHttpProgressAction::Execute()
    {
        if (this->Request->progressCallback!=0)
            this->Request->progressCallback->OnResponse(this->Request, this->Position, this->TotalLength, this->LengthKnown);
    }

    CurlHttpErrorAction::CurlHttpErrorAction(CurlHttpRequest* request, int errorCode, String errorMessage)
    {
        this->Request = request;
        this->ErrorCode = errorCode;
        this->ErrorMessage = errorMessage;
    }
    void CurlHttpErrorAction::Execute()
    {
        if (this->Request->errorCallback!=0)
            this->Request->errorCallback->OnResponse(this->Request, this->ErrorCode, this->ErrorMessage);
    }

    //------------------------------------------------------------

    HttpRequest* HttpRequest::Create()
    {
        if (!HttpInitialized) CurlHook.Init();
        return new CurlHttpRequest();
    }
}
