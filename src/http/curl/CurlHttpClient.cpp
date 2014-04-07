#include <Xli/HashMap.h>
#include <XliHttp/HttpClient.h>
#include <curl/curl.h>

namespace Xli
{
    CURLcode curlGlobalCode;


    class CurlHttpRequest : public HttpRequest
    {
    private:
        CURL* curlSession;
        Managed< HttpStateChangedHandler > stateChangedCallback;
        Managed< HttpProgressHandler > progressCallback;
        Managed< HttpTimeoutHandler > timeoutCallback;
        Managed< HttpErrorHandler > errorCallback;
        Managed< HttpStringPulledHandler > stringPulledCallback;
        Managed< HttpArrayPulledHandler > arrayPulledCallback;

        String url;
        int timeout;
        HttpMethodType method;
    public:
        CurlHttpRequest()
        {
            this->status = HttpUnsent;
            this->url = "";
            this->method = HttpGetMethod;
            this->timeout = 0;
        }
        CurlHttpRequest(String url, HttpMethodType method) 
        {
            this->status = HttpUnsent;
            this->url = url;
            this->method = method;
            this->timeout = 0;
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
            CURL* curl = curl_easy_init();
            if (!curl)
                XLI_THROW("CURL ERROR: Failed to create handle");
            
            CURLcode result = CURLE_OK;
            if (result == CURLE_OK) result = curl_easy_setopt(curl, CURLOPT_URL, url.DataPtr());
            if (result == CURLE_OK) result = curl_easy_setopt(curl, MethodToCurlCode(self->method), 1);
            if (result == CURLE_OK) result = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, self->timeout);
            if (result == CURLE_OK) result = curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);

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

            //progress callback
            if (this->progressCallback!=0 &&) 
            {
                result = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
                result = curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void*)this);
                result = curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgress);
            }
            

            if (result == CURLE_OK) result = curl_easy_perform(curl);

            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->EmitStateEvent();
            javaAsyncHandle = PlatformSpecific::AShim::SendHttpAsync(this);
        }

    private:
        virtual CURLcode MethodToCurlCode(HttpMethodType method)
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
        static int progressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow)
        {
            
        }
    };

    static int HttpInitialized = 0;
    static void InitAndroidHttp()
    {
        if (!HttpInitialized)
        {
            curlGlobalCode = curl_global_init(CURL_GLOBAL_ALL);
            HttpInitialized = 1;
        }
    }

    class CurlHttpClient : public HttpClient
    {
    public:
        CurlHttpClient() {}
        virtual ~CurlHttpClient() {}
        virtual HttpRequest* NewRequest()
        {
            return new CurlHttpRequest();
        }
    };

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized) InitAndroidHttp();
        return new CurlHttpClient();
    }
}
