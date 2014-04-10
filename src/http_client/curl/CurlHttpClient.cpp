#include <XliHttpClient/HttpClient.h>
#include "CurlHttpClient.h"
#include <Xli/HashMap.h>
#include <Xli/Mutex.h>
#include <curl/curl.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstring>

extern Xli::Window* GlobalWindow;

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
        int timeout;
        String url;
    public:
        Managed< HttpStateChangedHandler > stateChangedCallback;
        Managed< HttpProgressHandler > progressCallback;
        Managed< HttpTimeoutHandler > timeoutCallback;
        Managed< HttpErrorHandler > errorCallback;
        Managed< HttpStringPulledHandler > stringPulledCallback;
        Managed< HttpArrayPulledHandler > arrayPulledCallback;
        HashMap<String,String> headers;
        HashMap<String,String> responseHeaders;
        struct curl_slist* curlUploadHeaders;
        HttpRequestState status;
        HttpMethodType method;
        int responseStatus;
        void* uploadData;
        long uploadByteLength;
        long uploadPosition;
        String reasonPhrase;
        CURL* curlSession;
        pthread_t threadID;
        Array<UInt8> recievedData;
        String recievedString;
        bool contentAsString;
        bool dataReady;
        bool reading;
        // cross thread flags
        Mutex communicationMutex;
        bool unpause;
        bool abort;

        CurlHttpRequest()
        {
            this->status = HttpUnsent;
            this->url = "";
            this->method = HttpGetMethod;
            this->timeout = 0;
            this->dataReady = false;
            this->reading = false;
            this->unpause = false;
            this->abort = false;
            curlUploadHeaders=NULL;        
        }
        CurlHttpRequest(String url, HttpMethodType method)
        {
            this->status = HttpUnsent;
            this->url = url;
            this->method = method;
            this->timeout = 0;
            this->dataReady = false;
            this->reading = false;
            this->abort = false;
            curlUploadHeaders=NULL;
        }

        virtual ~CurlHttpRequest()
        {
        }

        virtual HttpRequestState GetStatus() const
        {
            return this->status;
        }
        virtual void EmitStateEvent()
        {
            GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpStateAction(this, this->status));
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

        virtual void Send(void* content, long byteLength)
        {
            if (this->status != HttpUnsent)
                XLI_THROW("HttpRequest->SetArrayPulledCallback(): Not in a valid state to set the virtual");

            //{TODO} Clarify when we copy data.
            this->uploadByteLength = byteLength;
            this->uploadData = malloc((size_t)byteLength);
            this->uploadPosition = 0;
            memcpy(content, this->uploadData, (size_t)byteLength);

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

            //progress callback
            if (this->progressCallback!=0)
            {
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_PROGRESSDATA, (void*)this);
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_PROGRESSFUNCTION, onProgress);
            }

            //Method specific options
            switch(method)
            {
            case HttpGetMethod:
                if (content!=0) result = CURLE_FAILED_INIT;
                break;
            case HttpPostMethod:
                if (content==0) result = CURLE_FAILED_INIT;
                if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDS, (void*)content);
                if (byteLength == -1)
                {
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDSIZE, -1);
                } else {
                    if (result == CURLE_OK) result = curl_easy_setopt(session, CURLOPT_POSTFIELDSIZE_LARGE, byteLength);
                }
                break;
            case HttpPutMethod:
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
                this->curlSession = session;
                pthread_attr_t attr;
                int rc;

                //{TODO} what attrs do we need set?
                //{TODO} when can we destroy the attrs?
                rc = pthread_attr_init(&attr);

                if (rc) {
                    GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpErrorAction(this, (int)CURLE_OK, "Xli: Http: Could not create request"));
                } else {
                    int threadError = pthread_create(&threadID, &attr, perform, (void*)this);
                    // {TODO} has some issues with compiling using the enums so am
                    //        using the int vars. Seems to be a clang issue.
                    switch (threadError)
                    {
                    case 35: //EAGAIN
                        GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpErrorAction(this, 0, "XliHttp: Not enough resources to crate thread"));
                        break;
                    case 22: //EINVAL
                        GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpErrorAction(this, 0, "XliHttp: Invalid settings in thread attributes."));
                        break;
                    case 1: //EPERM:
                        GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpErrorAction(this, 0, "XliHttp: Permission issue creating thread"));
                        break;
                    default:
                        if (threadError>0)
                            GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpErrorAction(this, 0, "XliHttp: Thread Error")); //{TODO} add error code here
                        break;
                    }
                    
                }
            } else {
                curl_easy_cleanup(session);
                GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpErrorAction(this, (int)result, "Xli: Http: Could not create request"));
            }
        }
        virtual void Send(String content)
        {
            this->Send((void*)content.DataPtr(), -1);
        }
        virtual void Send()
        {
            this->Send((void*)0, -1);
        }

        virtual void Abort()
        {
            if (this->status > 1 && this->status < 5 )
            {
                MutexLock lock(this->communicationMutex);
                this->abort = true;
            }
        }
        virtual void PullContentString()
        {
            if (this->status==HttpHeadersReceived)
            {
                this->contentAsString = true;
                MutexLock lock(this->communicationMutex);
                this->unpause = true;
            } else {
                GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpErrorAction(this, 0, "XliHttp: Can't pull content string, invalid state'"));
            }
        }
        virtual void PullContentArray()
        {
            if (this->status==HttpHeadersReceived)
            {
                this->contentAsString = false;
                MutexLock lock(this->communicationMutex);
                this->unpause = true;
            } else {
                GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpErrorAction(this, 0, "XliHttp: Can't pull content array, invalid state'"));
            }
        }

    private:
        static void* perform(void* requestVoid)
        {
            //signal state changed
            CurlHttpRequest* request = (CurlHttpRequest*)requestVoid;
            GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpStateAction(request, HttpSent));

            //start the curl request. curl_easy_perform will block
            CURLcode error = curl_easy_perform(request->curlSession);            
            
            if (error != 0)
            {
                switch(error)
                {
                case CURLE_OPERATION_TIMEDOUT : onTimeout(request); break;
                default:
                    String finalMessage = Xli::String("XliHttp: UNHANDLED CURL ERROR: ");
                    finalMessage += CurlErrorToString(error);
                    GlobalWindow->EnqueueCrossThreadEvent(new CTError(finalMessage));
                    break;
                };
            }

            //{TODO} cleanup this thread only and then call back to main thread to clean up there
            //       -how?
            return 0;
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

        static size_t onDataRecieved(char* ptr, size_t size, size_t nmemb, void* userdata)
        {
            CurlHttpRequest* request = (CurlHttpRequest*)userdata;
            size_t actualBytesRead = 0;

            // {TODO} saying we have the headers at this point. Check the validity

            if (request->reading == true)
            {
                request->dataReady = true;
                if (request->contentAsString) {
                    actualBytesRead = onStringDataRecieved(request, ptr, (size * nmemb));
                } else {
                    actualBytesRead = onByteDataRecieved(request, ptr, (size * nmemb));
                }
            } else {
                request->dataReady = true;
                actualBytesRead = CURL_WRITEFUNC_PAUSE;
                if ((int)request->status < (int)HttpHeadersReceived)
                    GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpStateAction(request, HttpHeadersReceived));
            }
            //{TODO} Is this correct? check the specific methods
            return actualBytesRead;
        }
        static size_t onStringDataRecieved(CurlHttpRequest* request, char* ptr, size_t bytesRecieved)
        {
            request->recievedString.Append((char*)ptr, bytesRecieved);
            return bytesRecieved;
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
            if (request->reading)
            {
                if (dlnow)
                    GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpProgressAction(request, (long)dlnow, (long)dltotal, (dltotal > 0)));
            } else {
                MutexLock lock(request->communicationMutex);
                if (request->unpause)
                {
                    request->reading=true;
                    GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpStateAction(request, HttpLoading));
                    curl_easy_pause(request->curlSession, CURLPAUSE_CONT);
                }
            }
            return 0;
        }
        static void onTimeout(CurlHttpRequest* request)
        {
            GlobalWindow->EnqueueCrossThreadEvent(new CurlHttpTimeoutAction(request));
        }
        static void onStringPulled()
        {

        }
        static void onArrayPulled()
        {
        }
    };

    //------------------------------------------------------------

    CurlHttpStateAction::CurlHttpStateAction(CurlHttpRequest* request, HttpRequestState status)
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
                if (!this->Request->contentAsString)
                {
                    if (this->Request->recievedData.Length() && this->Request->arrayPulledCallback)
                    {
                        this->Request->arrayPulledCallback->OnResponse(this->Request, this->Request->recievedData.DataPtr(), this->Request->recievedData.Length());
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
        if (!HttpInitialized) InitCurlHttp();
        return new CurlHttpRequest();
    }
}
