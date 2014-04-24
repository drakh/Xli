#include <XliHttpClient/HttpClient.h>
#include "CurlHttpClient.h"
#include <Xli/HashMap.h>
#include <Xli/Mutex.h>
#include <curl/curl.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstring>

namespace Xli
{
    static int HttpInitialized = 0;
    static CURLcode CurlGlobalCode;
    static void InitCurlHttp()
    {
        if (!HttpInitialized)
        {
            CurlGlobalCode = curl_global_init(CURL_GLOBAL_ALL);
            HttpInitialized = 1;
        }
    }
    static void ShutdownCurlHttp()
    {
        curl_global_cleanup();
    }

    class CurlHttpRequest : public HttpRequest
    {
    private:
        HttpClient* client;

        String method;
        String url;
        int timeout;
        HttpRequestState state;

        HashMap<String,String> headers;
        struct curl_slist* curlUploadHeaders;

        CURL* curlSession;
        void* uploadData;
        long uploadByteLength;
        long uploadPosition;

        HashMap<String,String> responseHeaders;
        Array<UInt8> recievedData;
        int responseStatus;

    public:

        CurlHttpRequest(HttpClient* client, String url, HttpMethod method)
        {
            this->client = client;
            this->state = HttpRequestStateUnsent;
            this->url = url;
            this->method = method;
            this->timeout = 0;
            this->abort = false;
            curlUploadHeaders=NULL;
        }

        virtual ~CurlHttpRequest()
        {
        }

        virtual String GetMethod() const { return method; }
        virtual String GetUrl() const { return url; }
        virtual HttpRequestState GetState() const { return state; }
        virtual int GetTimeout() const { return timeout; }
        virtual void SetTimeout(int timeout)
        {
            if (state == HttpRequestStateUnsent)
            {
                this->timeout = timeout;
            } else {
                XLI_THROW("HttpRequest->SetTimeout(): Not in a valid state to set the timeout");
            }
        }

        virtual void SetHeader(const String& key, const String& value)
        {
            if (state == HttpRequestStateUnsent)
            {
                headers.Add(key,value);
            } else {
                XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
            }
        }
        virtual void RemoveHeader(const String& key)
        {
            if (state == HttpRequestStateUnsent)
            {
                headers.Remove(key);
            } else {
                XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
            }
        }
        virtual int HeadersBegin() const { return headers.Begin(); }
        virtual int HeadersEnd() const { return headers.End(); }
        virtual int HeadersNext(int n) const { return headers.Next(n); }
        virtual String GetHeaderKey(int n) const { return headers.GetKey(n); }
        virtual String GetHeaderValue(int n) const { return headers.GetValue(n); }

        virtual int GetResponseHeaderCount() const
        {
            if (state == HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Count();
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderCount(): Not in a valid state to get the response header count");
            }
        }
        virtual int ResponseHeadersBegin() const
        {
            if (state == HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Begin();
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderBegin(): Not in a valid state to get the response header iterator");
            }
        }
        virtual int ResponseHeadersNext(int n) const
        {
            if (state == HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Next(n);
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderNext(): Not in a valid state to get the next response header");
            }
        }
        virtual int ResponseHeadersEnd() const
        {
            if (state == HttpRequestStateHeadersReceived)
            {
                return responseHeaders.End();
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderEnd(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeaderKey(int n) const
        {
            if (state == HttpRequestStateHeadersReceived)
            {
                return responseHeaders.GetKey(n);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeaderValue(int n) const
        {
            if (state == HttpRequestStateHeadersReceived)
            {
                return responseHeaders.GetValue(n);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
            }
        }

        virtual int GetResponseStatus() const
        {
            if (state == HttpRequestStateHeadersReceived) // {TODO} is this the correct state?
            {
                return responseStatus;
            } else {
                XLI_THROW("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
            }
        }

        virtual bool TryGetResponseHeader(const String& key, String& result) const
        {
            if (state == HttpRequestStateHeadersReceived)
            {
                return responseHeaders.TryGetValue(key, result);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeader(): Not in a valid state to get the response header");
            }
        }


        virtual DataAccessor* GetResponseBody() const
        {
            &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&;
            // if (state == HttpRequestStateDone)
            // {
            //     return (DataAccessor*)responseBody;
            // } else {
            //     XLI_THROW("HttpRequest->GetResponseBody(): Not in a valid state to get the response body");
            // }
        }


        virtual void SendASync(void* content, long byteLength)
        {
            if (this->state != HttpRequestStateUnsent)
                XLI_THROW("HttpRequest->SetArrayPulledCallback(): Not in a valid state to send");

            this->uploadByteLength = byteLength;
            this->uploadData = content;
            this->uploadPosition = 0;

            CURL* session = curl_easy_init();
            if (!session)
                XLI_THROW("CURL ERROR: Failed to create handle");

            CURLcode result = CURLE_OK;
            if (result == CURLE_OK) result = curl_easy_setopt(session, methodToCurlOption(this->method), 1);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_URL, url.DataPtr());
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_CONNECTTIMEOUT, this->timeout);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_AUTOREFERER, 1);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_HEADERFUNCTION, onHeaderRecieved);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_HEADERDATA, (void*)this);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, onDataRecieved);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_WRITEDATA, (void*)this);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_PROGRESSDATA, (void*)this);
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_PROGRESSFUNCTION, onProgress);

            //Method specific options
            switch(method)
            {
            case HttpMethodGet:
                if (content!=0) result = CURLE_FAILED_INIT;
                break;
            case HttpMethodPost:
                if (content==0) result = CURLE_FAILED_INIT;
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDS, (void*)content);
                if (byteLength == -1)
                {
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDSIZE, -1);
                } else {
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDSIZE_LARGE, byteLength);
                }
                break;
            case HttpMethodPut:
                if (content==0) result = CURLE_FAILED_INIT;
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_READFUNCTION, onDataUpload);
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_READDATA, (void*)this);
                break;
            default:
                if (content==0) result = CURLE_FAILED_INIT;
                break;
            };

            //Add headers
            int i = this->HeadersBegin();
            String header;
            while (i != this->HeadersEnd())
            {
                header = "";
                header.Append(this->GetHeaderKeyN(i));
                header.Append(":");
                header.Append(this->GetHeaderValueN(i));
                this->curlUploadHeaders = curl_slist_append(this->curlUploadHeaders, header.DataPtr());
                i = this->HeadersNext(i);
            }           
            curl_easy_setopt(session, CURLOPT_HTTPHEADER, this->curlUploadHeaders);
            //{TODO} Add following to cleanup: curl_slist_free_all(this->curlUploadHeaders);

            if (result == CURLE_OK)
            {
                //Add to the client's multi-session
                client->AddSession(session);
            } else {
                curl_easy_cleanup(session);
                this->client->EnqueueAction(new CurlHttpErrorAction(this, (int)result, "Xli: Http: Could not create request"));
            }
        }
        virtual void SendASync(String content)
        {
            this->SendASync((void*)content.DataPtr(), -1);
        }
        virtual void SendASync()
        {
            this->SendASync((void*)0, -1);
        }

        virtual void Abort()
        {
            // {TODO} what are we aborting? needs to work for upload and download
            abort = true;
        }

    private:
        static size_t onDataUpload( void *ptr, size_t size, size_t nmemb, void *userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;
            size_t maxCopyLength = (size * nmemb);
            size_t copyLength = 0;
            if (maxCopyLength > 0)
            {
                copyLength = (request->uploadByteLength - request->uploadPosition);
                if (copyLength > maxCopyLength) copyLength = maxCopyLength;
                if (copyLength>0)
                {
                    memcpy(request->uploadData, ptr, copyLength);
                    request->uploadPosition += copyLength;
                }
            }
            return copyLength;
        }

        static size_t onHeaderRecieved(void *ptr, size_t size, size_t nmemb, void *userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;
            size_t bytesPulled = (size * nmemb);
            if (bytesPulled > 0)
            {
                String fullHeader((char*)ptr, bytesPulled);
                int splitPos = fullHeader.IndexOf(':');
                if (splitPos > 0)
                {
                    request->responseHeaders.Add(fullHeader.Substring(0, splitPos-1), fullHeader.Substring(splitPos+1));
                } else {
                    request->responseHeaders.Add(fullHeader, "");
                }
            }
            return bytesPulled;
        }

        static size_t onDataRecieved(char* ptr, size_t size, size_t nmemb, void* userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;
            size_t actualBytesRead = 0;

            // {TODO} saying we have the headers at this point. Check the validity

            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(request);

            actualBytesRead = onByteDataRecieved(request, ptr, (size * nmemb));

            //{TODO} Is this correct? check the specific methods
            return actualBytesRead;
        }
        static size_t onByteDataRecieved(CurlHttpRequest* request, char* ptr, size_t bytesRecieved)
        {
            request->recievedData.AddRange((UInt8*)ptr, bytesRecieved);
            return bytesRecieved;
        }
        static int onProgress(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)clientp;
            if (request->abort) 
            {
                return CURLE_ABORTED_BY_CALLBACK;
            }
            if (dlnow){
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0) eh->OnRequestProgress(request, dlnow, dltotal, (dltotal > 0), HttpTransferDirection_DOWNLOAD);
            }
            if (ulnow){
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0) eh->OnRequestProgress(request, ulnow, ultotal, (ultotal > 0), HttpTransferDirection_UPLOAD);
            }
            return 0;
        }
        static void onTimeout(CurlHttpRequest* request)
        {
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestTimeout(request);
        }
        static void onStringPulled()
        {

        }
        static void onArrayPulled()
        {
        }
    };

    //------------------------------------------------------------

    CurlHttpClient::CurlHttpClient()
    {
        multiSession = curl_multi_init();
    }

    CurlHttpClient::~CurlHttpClient()
    {
        // {TODO} cleanup multisession here
        // CURLMcode curl_multi_cleanup( CURLM *multi_handle );
    }

    CurlHttpRequest* CurlHttpClient::CreateRequest(const String& method, const String& url)
    {
        return new CurlHttpRequest(this, url, method);
    }

    void CurlHttpClient::SetEventHandler(HttpEventHandler* eventHandler) 
    {
        this->eventHandler = eventHandler;
    }

    HttpEventHandler* CurlHttpClient::GetEventHandler() 
    {
        return eventHandler;
    }

    void CurlHttpClient::AddSession(CURL* session)
    {
        CURLMcode result = curl_multi_add_handle(multiSession, session);        
    }

    void CurlHttpClient::Update()
    {
        
    }

    HttpClient* HttpClient::Create()
    {
        return new CurlHttpClient();
    }
}
