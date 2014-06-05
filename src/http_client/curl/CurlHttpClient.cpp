#include <Xli/ArrayStream.h>
#include <Xli/BufferStream.h>
#include <Xli/Buffer.h>
#include <Xli/HashMap.h>

#include <XliHttpClient/HttpClient.h>
#include "CurlHttpClient.h"

#include <curl/curl.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstring>
#include <stdlib.h>

namespace Xli
{
    static int HttpInitialized = 0;
    static CURLcode CurlGlobalCode;
    static void ShutdownCurlHttp()
    {
        // Assumes clients have cleaned up up the handles they own
        curl_global_cleanup();
    }
    static void InitCurlHttp()
    {
        if (!HttpInitialized)
        {
            CurlGlobalCode = curl_global_init(CURL_GLOBAL_ALL);
            atexit(Xli::ShutdownCurlHttp);
            HttpInitialized = 1;
        }
    }    

    class SessionMap
    {
    public:
        static HashMap<HttpRequest*, CURL*> abortedTable;
        static bool IsAborted(HttpRequest* requestHandle)
        {
            return abortedTable.ContainsKey(requestHandle);
        }
        static CURL* PopSession(HttpRequest* requestHandle)
        {
            CURL* session;
            bool found = abortedTable.TryGetValue(requestHandle, session);
            if (found)
            {
                abortedTable.Remove(requestHandle);
                return session;
            } else {
                XLI_THROW("XLIHTTP: PopSession - request not found in abortedTable");
            }
        }        
        static void MarkAborted(HttpRequest* requestHandle, CURL* session)
        {
            if (!abortedTable.ContainsKey(requestHandle))
                abortedTable.Add(requestHandle, session);
        }
    };
    HashMap<HttpRequest*, CURL*> SessionMap::abortedTable;
    
    class CurlHttpRequest : public HttpRequest
    {
    private:
        CurlHttpClient* client;

        HttpRequestState state;

        String url;
        String method;
        int timeout;
        bool verifyHost;

        HashMap<String,String> headers;
        struct curl_slist* curlUploadHeaders;
        CURL* curlSession;
        bool requestOwnsUploadData;

        bool aborted;

        Managed<BufferStream> uploadBuffer;

        HashMap<String,String> responseHeaders;
        Managed< ArrayStream > responseBody;
        Managed< BufferReference > responseBodyRef;
        int responseStatus;

    public:

        CurlHttpRequest(CurlHttpClient* client, String url, String method)
        {
            this->client = client;
            this->state = HttpRequestStateOpened;
            this->url = url;
            this->method = method;
            this->timeout = 0;
            this->curlUploadHeaders=NULL;
            this->responseBody = new ArrayStream(1);
            this->aborted = false;
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
        }

        virtual ~CurlHttpRequest()
        {
            Abort();
            curl_slist_free_all(curlUploadHeaders);            
            // {TODO} free upload headers
        }

        virtual String GetMethod() const { return method; }
        virtual String GetUrl() const { return url; }
        virtual HttpRequestState GetState() const { return state; }
        virtual int GetTimeout() const { return timeout; }
        virtual void SetTimeout(int timeout)
        {
            if (state <= HttpRequestStateOpened)
            {
                this->timeout = timeout;
            } else {
                XLI_THROW("HttpRequest->SetTimeout(): Not in a valid state to set the timeout");
            }
        }
        virtual bool GetVerifyHost() const { return verifyHost; }
        virtual void SetVerifyHost(bool verify)
        {
            if (state <= HttpRequestStateOpened)
            {
                verifyHost = verify;
            } else {
                XLI_THROW("HttpRequest->SetTimeout(): Not in a valid state to set the verify host flag");
            }
        }
        virtual void SetHeader(const String& key, const String& value)
        {
            if (state == HttpRequestStateOpened)
            {
                headers.Add(key,value);
            } else {
                XLI_THROW("HttpRequest->SetHeader(): Not in a valid state to set a header");
            }
        }
        virtual void RemoveHeader(const String& key)
        {
            if (state == HttpRequestStateOpened)
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

        virtual CURLcode SetCurlRequestOptions(CURL* session, const void* content, int byteLength)
        {
            CURLcode result = CURLE_OK;
            // set method option
            if (method == "GET") 
            {
                result = curl_easy_setopt(session, CURLOPT_HTTPGET, 1);
            } else if (method == "POST") {
                result = curl_easy_setopt(session, CURLOPT_POST, 1);
            } else if (method == "PUT") {
                result = curl_easy_setopt(session, CURLOPT_UPLOAD, 1);
            } else if (method == "OPTIONS" || method == "HEAD" || method == "TRACE" || method == "DELETE") {
                curl_easy_setopt(session, CURLOPT_CUSTOMREQUEST, method.DataPtr()); 
            } else {
                return CURLE_FAILED_INIT;
            }
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_URL, url.DataPtr());
            if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_CONNECTTIMEOUT, timeout);
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
            if (method ==  "GET")
            {
                if (content!=0 && byteLength>0) result = CURLE_FAILED_INIT;
            } else if (method == "HEAD") {
                if (content!=0 && byteLength>0) result = CURLE_FAILED_INIT;
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_NOBODY, 1);
            } else if (method == "POST") {                
                if (byteLength <= 0)
                {
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDSIZE, -1);
                } else {
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDS, (void*)content);
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDSIZE_LARGE, byteLength);
                }
            } else if (method == "PUT") {
                if (content==0 || byteLength <= 0) result = CURLE_FAILED_INIT;
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_READFUNCTION, onDataUpload);
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_READDATA, (void*)this);
            };

            return result;
        }
        
        virtual void SendAsync(const void* content, int byteLength)
        {
            if (state != HttpRequestStateOpened)
            {
                //XLI_THROW("HttpRequest->SetArrayPulledCallback(): Not in a valid state to send");
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0 && !aborted) eh->OnRequestError(this);
                return;
            }

            if (content!=0 && byteLength>0)
            {
                // const_cast is safe here are bufferstream is set readonly
                uploadBuffer = new BufferStream(new BufferPointer(const_cast<void*>(content), byteLength, requestOwnsUploadData), true, false);
            }

            //Create session
            CURL* session = curl_easy_init();
            if (!session)
            {
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0 && !aborted) eh->OnRequestError(this);
                return;
            } 

            // Set curl options
            CURLcode result = SetCurlRequestOptions(session, content, byteLength);

            //Add headers
            int i = HeadersBegin();
            String header;
            while (i != HeadersEnd())
            {
                header = "";
                header.Append(GetHeaderKey(i));
                header.Append(":");
                header.Append(GetHeaderValue(i));
                curlUploadHeaders = curl_slist_append(curlUploadHeaders, header.DataPtr());
                i = HeadersNext(i);
            }
            curl_easy_setopt(session, CURLOPT_HTTPHEADER, curlUploadHeaders);

            if (result == CURLE_OK)
            {
                state = HttpRequestStateSent;

                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0 && !aborted) eh->OnRequestStateChanged(this);

                curlSession = session;
                client->AddSession(session, this);
            } else {
                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0 && !aborted) eh->OnRequestError(this);
            }
        }
        virtual void SendAsync(const String& content)
        {
            requestOwnsUploadData = true;
            void* raw = malloc(content.Length());
            memcpy(raw, content.DataPtr(), content.Length());                
            SendAsync(raw, content.Length());
        }
        virtual void SendAsync()
        {
            SendAsync((void*)0, -1);
        }

        virtual void Abort()
        {
            if (!aborted)
            {
                // {TODO} Can we guarentee instant termination?
                //        If so we dont need to worry about post destroy callbacks
                client->RemoveSession(curlSession);
                SessionMap::MarkAborted(this, curlSession);
                curlSession = 0;
                aborted = true;

                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0) eh->OnRequestAborted(this);
            }
        }

        virtual int GetResponseHeaderCount() const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Count();
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderCount(): Not in a valid state to get the response header count");
            }
        }
        virtual int ResponseHeadersBegin() const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Begin();
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderBegin(): Not in a valid state to get the response header iterator");
            }
        }
        virtual int ResponseHeadersNext(int n) const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.Next(n);
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderNext(): Not in a valid state to get the next response header");
            }
        }
        virtual int ResponseHeadersEnd() const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.End();
            } else {
                XLI_THROW("HttpRequest->ResponseHeaderEnd(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeaderKey(int n) const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.GetKey(n);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
            }
        }
        virtual String GetResponseHeaderValue(int n) const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.GetValue(n);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
            }
        }

        virtual int GetResponseStatus() const
        {
            if (state >= HttpRequestStateHeadersReceived) // {TODO} is this the correct state?
            {
                return responseStatus;
            } else {
                XLI_THROW("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
            }
        }

        virtual bool TryGetResponseHeader(const String& key, String& result) const
        {
            if (state >= HttpRequestStateHeadersReceived)
            {
                return responseHeaders.TryGetValue(key, result);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeader(): Not in a valid state to get the response header");
            }
        }

        virtual DataAccessor* GetResponseBody() const
        {
            if (state == HttpRequestStateDone)
            {
                return (DataAccessor*)responseBodyRef.Get();
            } else {
                XLI_THROW("HttpRequest->GetResponseBody(): Not in a valid state to get the response body");
            }
        }

        virtual void onDone()
        {
            state = HttpRequestStateDone;

            if (aborted) //{TODO} spin this off?
            {
                CURL* session = SessionMap::PopSession(this);
                curl_easy_cleanup(session);
            }

            responseBodyRef = new BufferReference((void*)responseBody->GetDataPtr(), responseBody->GetLength(), (Object*)responseBody.Get());

            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0 && !aborted) eh->OnRequestStateChanged(this);
        }

    private:
        static size_t onDataUpload( void *ptr, size_t size, size_t nmemb, void *userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;

            // if the session is aborted then we want to skip this. We cant say
            // we did the job though so we pass back 0 and accept this may indicate
            // that this method caused the problem
            if (SessionMap::IsAborted(request)) return 0;
            
            size_t maxCopyLength = (size * nmemb);
            size_t copyLength = 0;
            if (maxCopyLength > 0 && !request->uploadBuffer->AtEnd())
            {
                copyLength = request->uploadBuffer->Read(ptr, 1, maxCopyLength);
            }
            return copyLength;
        }

        static size_t onHeaderRecieved(void *ptr, size_t size, size_t nmemb, void *userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;

            // if the session is aborted then we want to skip this but not 
            // cause curl to think this method caused the problem.
            if (SessionMap::IsAborted(request)) return (size * nmemb);

            size_t bytesPulled = (size * nmemb);
            if (bytesPulled > 0)
            {
                String fullHeader((char*)ptr, bytesPulled);
                int splitPos = fullHeader.IndexOf(':');
                if (splitPos > 0)
                {
                    String key = fullHeader.Substring(0, splitPos);
                    String val = fullHeader.Substring(splitPos+1);
                    if (!request->responseHeaders.ContainsKey(key)) 
                        request->responseHeaders.Add(key, val);
                } else {
                    if (!request->responseHeaders.ContainsKey(fullHeader)) 
                        request->responseHeaders.Add(fullHeader, "");
                }
            }
            return bytesPulled;
        }

        static size_t onDataRecieved(char* ptr, size_t size, size_t nmemb, void* userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;

            // if the session is aborted then we want to skip this but not 
            // cause curl to think this method caused the problem.
            if (SessionMap::IsAborted(request)) return (size * nmemb);

            size_t bytesRead = 0;

            // {TODO} saying we have the headers at this point. Check the validity
            request->state = HttpRequestStateHeadersReceived;
            HttpEventHandler* eh = request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(request);

            bytesRead = (size * nmemb);
            request->responseBody->Write((UInt8*)ptr, 1, bytesRead);

            return bytesRead;
        }

        static int onProgress(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)clientp;

            if (SessionMap::IsAborted(request))
            {                
                // CURL* session = SessionMap::PopSession(request);{TODO} we need to know what gets called
                // curl_easy_cleanup(session);
                return CURLE_ABORTED_BY_CALLBACK;
            }
            if (dlnow && (request->state == HttpRequestStateLoading))
            {
                HttpEventHandler* eh = request->client->GetEventHandler();
                if (eh!=0) eh->OnRequestProgress(request, dlnow, dltotal, (dltotal > 0));
            }
            if (ulnow && (request->state == HttpRequestStateSent))
            {
                HttpEventHandler* eh = request->client->GetEventHandler();
                if (eh!=0) eh->OnRequestProgress(request, ulnow, ultotal, (ultotal > 0));
            }
            return 0;
        }
        static void onTimeout(CurlHttpRequest* request) // {TODO} this is never used...the fuck?
        {

            HttpEventHandler* eh = request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestTimeout(request);
        }

        //{TODO} What happens if there is an error? Do we get a callback? We need this to clean up
    };

    //------------------------------------------------------------


    CurlHttpClient::CurlHttpClient()
    {
        multiSession = curl_multi_init();
    }

    CurlHttpClient::~CurlHttpClient()
    {
        int i = sessionTable.Begin();
        while (i != sessionTable.End())
        {
            CurlHttpRequest* request = sessionTable.GetValue(i);
            request->Abort(); // abort also calls RemoveSession in the client
            i = sessionTable.Next(i);
        }
        //{TODO} find all curlcodes in this file and add checks for them
        CURLMcode result = curl_multi_cleanup(multiSession); 
    }

    HttpRequest* CurlHttpClient::CreateRequest(const String& method, const String& url)
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

    void CurlHttpClient::AddSession(CURL* session, CurlHttpRequest* request)
    {
        sessionTable.Add(session, request);
        CURLMcode result = curl_multi_add_handle(multiSession, session);
    }

    void CurlHttpClient::RemoveSession(CURL* session)
    {
        if (sessionTable.ContainsKey(session))
        {
            sessionTable.Remove(session);
            curl_multi_remove_handle(multiSession, session);            
        } else {
            XLI_THROW("Could not remove HttpRequest session as none found in session table");
        }
    }

    void CurlHttpClient::Update()
    {
        int msgRemaining;
        int runningHandles;
        CURLMsg* msg;
        CURL* session;
        CurlHttpRequest* request;
        CURLMSG messageType;

        CURLMcode pCode = curl_multi_perform(multiSession, &runningHandles);

        while (NULL != (msg = curl_multi_info_read(multiSession, &msgRemaining)))
        {
            session = msg->easy_handle;
            request = sessionTable[session];
            messageType = msg->msg;
            if (messageType == CURLMSG_DONE)
            {
                // we need to find errors here
                request->onDone();
            } else {
                HttpEventHandler* eh = this->GetEventHandler();
                if (eh!=0) eh->OnRequestError(request);
            }
        }
    }

    HttpClient* HttpClient::Create()
    {
        if (!HttpInitialized) InitCurlHttp();
        return new CurlHttpClient();
    }
}
