#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#include <Xli/PlatformSpecific/iOS.h>
#include <XliHttpClient/HttpClient.h>
#include <Xli/HashMap.h>
#include <CFNetwork/CFNetwork.h>
#include <CFNetwork/CFHTTPStream.h>
#include <iostream>
#include <fstream>

namespace Xli
{
    class CHttpRequest : public HttpRequest
    {
    private:
        HttpClient* client;
        HashMap<String,String> headers;
        HashMap<String,String> responseHeaders;
        HttpRequestState state;
        String method;
        int responseStatus;
        String reasonPhrase;
        String url;
        int timeout;

        CFHTTPMessageRef cachedRequestMessage;
        CFReadStreamRef cachedReadStream;
        CFDataRef cachedUploadData;
        bool errored;
        
        bool dataReady;
        bool reading;
        int readPosition;

        CFWriteStreamRef cachedContentStream;
        Buffer* responseBody;

        virtual void getContentArray() 
        {            
            if (state == HttpRequestStateDone && !errored)
            {
                CFStringRef prop = CFSTR("kCFStreamPropertyDataWritten");
                CFDataRef streamDataHandle = (CFDataRef)CFWriteStreamCopyProperty(cachedContentStream, prop);
                long len = CFDataGetLength(streamDataHandle);
                readPosition = len;
                
                responseBody = Buffer::Create((int)len);
                CFDataGetBytes(streamDataHandle, CFRangeMake(0,len), (UInt8*)responseBody->GetDataPtr());

                CFWriteStreamClose(cachedContentStream);
                CFRelease(cachedContentStream);
                CFRelease(streamDataHandle);//{TODO} test this
                cachedContentStream = 0;
            }
            NSLog(@"Cant get content array"); // {TODO} error needed here
        }
    public:

        CHttpRequest(HttpClient* client, String url, String method) 
        {
            this->client = client;
            this->state = HttpRequestStateUnsent;
            this->url = url;
            this->method = method;
            this->timeout = 0;
            this->readPosition = 0;
            this->reading = false;
            this->cachedContentStream = 0;
            this->cachedReadStream = 0;
            this->cachedRequestMessage = 0;
        }

        virtual ~CHttpRequest()
        {
            state = HttpRequestStateDone;
            
            if (cachedRequestMessage != 0)
                CFRelease(cachedRequestMessage);
            if (cachedReadStream!=0)
            {
                CFReadStreamClose(cachedReadStream);
                CFRelease(cachedReadStream);
            }
            if (cachedReadStream!=0)
                CFRelease(cachedReadStream);
            if (cachedContentStream!=0)
                CFRelease(cachedContentStream);
            if (cachedUploadData!=0)
                CFRelease(cachedUploadData);
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
            if (state == HttpRequestStateDone)
            {
                return (DataAccessor*)responseBody;
            } else {
                XLI_THROW("HttpRequest->GetResponseBody(): Not in a valid state to get the response body");
            }
        }

        virtual void SendAsync(const void* content, int byteLength)
        {
            if (this->state != HttpRequestStateUnsent) return;
            
            CFStringRef nUrlStr = CFStringCreateWithCString(kCFAllocatorDefault, this->url.DataPtr(), kCFStringEncodingUTF8);
            CFURLRef nUrl = CFURLCreateWithString(kCFAllocatorDefault, nUrlStr, NULL);
            CFStringRef nMethod = CFStringCreateWithCString(kCFAllocatorDefault, this->method.DataPtr(), kCFStringEncodingUTF8);
            CFHTTPMessageRef nHttpReq = CFHTTPMessageCreateRequest(kCFAllocatorDefault, nMethod, nUrl, kCFHTTPVersion1_1);
            
            if (byteLength>0)
            {
                CFDataRef bodyData = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, (const UInt8*)content, (CFIndex)byteLength, kCFAllocatorDefault);
                CFHTTPMessageSetBody(nHttpReq, bodyData);
                this->cachedUploadData = bodyData;
            } else {
                cachedUploadData = 0;
            }
            
            SetCFHeaders(nHttpReq);
            
            cachedReadStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, nHttpReq);
            CFNumberRef nTimeout = CFNumberCreate(kCFAllocatorDefault, kCFNumberDoubleType, &this->timeout);
            CFReadStreamSetProperty(this->cachedReadStream, CFSTR("_kCFStreamPropertyReadTimeout"), nTimeout); //{TODO} doco is flakey on this, does it work?
            CFReadStreamSetProperty(this->cachedReadStream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue);
            
            this->cachedRequestMessage = nHttpReq;
            
            CFOptionFlags optEvents = kCFStreamEventOpenCompleted|kCFStreamEventHasBytesAvailable|kCFStreamEventErrorOccurred|kCFStreamEventEndEncountered;
            
            CFStreamClientContext context = {0, this, NULL, NULL, NULL };
            
            CFReadStreamSetClient(cachedReadStream, optEvents, AsyncCallback, &context);
            CFReadStreamScheduleWithRunLoop( cachedReadStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes );
            CFReadStreamOpen( cachedReadStream );
            
            this->state = HttpRequestStateSent;
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
            
            CFRelease(nUrlStr);
            CFRelease(nMethod);
            CFRelease(nUrl);
        }

        virtual void SendAsync(const String& content)
        {
            if (this->state != HttpRequestStateUnsent) return;
            
            CFStringRef nUrlStr = CFStringCreateWithCString(kCFAllocatorDefault, this->url.DataPtr(), kCFStringEncodingUTF8);
            CFURLRef nUrl = CFURLCreateWithString(kCFAllocatorDefault, nUrlStr, NULL);
            CFStringRef nMethod = CFStringCreateWithCString(kCFAllocatorDefault, this->method.DataPtr(), kCFStringEncodingUTF8);
            CFHTTPMessageRef nHttpReq = CFHTTPMessageCreateRequest(kCFAllocatorDefault, nMethod, nUrl, kCFHTTPVersion1_1);
            
            if (content.Length()>0)
            {
                CFStringRef data = CFStringCreateWithCString(kCFAllocatorDefault, content.DataPtr(), kCFStringEncodingUTF8);
                CFDataRef bodyData = CFStringCreateExternalRepresentation(kCFAllocatorDefault, data, kCFStringEncodingUTF8, 0);
                CFHTTPMessageSetBody(nHttpReq, bodyData);
                this->cachedUploadData = bodyData;
            } else {
                this->cachedUploadData = 0;
            }
            
            SetCFHeaders(nHttpReq);
            
            this->cachedReadStream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, nHttpReq);
            CFReadStreamSetProperty(this->cachedReadStream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue);
            
            this->cachedRequestMessage = nHttpReq;
            
            CFOptionFlags optEvents = kCFStreamEventOpenCompleted|kCFStreamEventHasBytesAvailable|kCFStreamEventErrorOccurred|kCFStreamEventEndEncountered;
            
            CFStreamClientContext context = {0, this, NULL, NULL, NULL };
            
            CFReadStreamSetClient(cachedReadStream, optEvents, AsyncCallback, &context);
            CFReadStreamScheduleWithRunLoop( cachedReadStream, CFRunLoopGetCurrent(), kCFRunLoopCommonModes );
            CFReadStreamOpen( cachedReadStream );
            
            this->state = HttpRequestStateSent;
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
            
            CFRelease(nUrlStr);
            CFRelease(nMethod);
            CFRelease(nUrl);
        }

        virtual void SendAsync()
        {
            this->SendAsync("");
        }

        virtual void SetCFHeaders(CFHTTPMessageRef message)
        {
            if (this->state == HttpRequestStateUnsent)
            {
                int i = this->headers.Begin();
                while (i != this->headers.End())
                {
                    CFStringRef nKey = CFStringCreateWithCString(kCFAllocatorDefault, this->headers.GetKey(i).DataPtr(), kCFStringEncodingUTF8);
                    CFStringRef nVal = CFStringCreateWithCString(kCFAllocatorDefault, this->headers.GetValue(i).DataPtr(), kCFStringEncodingUTF8);

                    CFHTTPMessageSetHeaderFieldValue(message, nKey, nVal);

                    CFRelease(nKey);
                    CFRelease(nVal);
                    i = this->headers.Next(i);
                }
            }
        }

        virtual void Abort()
        {
            if (this->state > 1 && this->state < 5 )
            this->state = HttpRequestStateDone; //{TODO} how does statechanged know if it was successful?
            
            if (cachedReadStream!=0)
            {
                CFReadStreamClose(this->cachedReadStream);
                CFRelease(this->cachedReadStream);
            }
            if (cachedRequestMessage != 0)
                CFRelease(this->cachedRequestMessage);
            if (cachedReadStream!=0)
                CFRelease(this->cachedReadStream);
            if (cachedContentStream!=0)
                CFRelease(this->cachedContentStream);
            if (cachedUploadData!=0)
                CFRelease(this->cachedUploadData);
            
            HttpEventHandler* eh = client->GetEventHandler();
            if (eh!=0) eh->OnRequestStateChanged(this);
        }

        virtual void StartDownload()
        {    
            if ((this->state==HttpRequestStateHeadersReceived) && (this->cachedContentStream == 0))
            {
                this->state = HttpRequestStateLoading;
                this->reading = true;

                HttpEventHandler* eh = client->GetEventHandler();
                if (eh!=0) eh->OnRequestStateChanged(this);
          
                this->cachedContentStream = CFWriteStreamCreateWithAllocatedBuffers(kCFAllocatorDefault, kCFAllocatorDefault);
                CFWriteStreamOpen(this->cachedContentStream);
                if (dataReady) OnByteDataRecieved(this, cachedReadStream, NULL);
            } else {
                NSLog(@"Cant pull content array"); //{TODO} proper error here
            }
        }

        virtual void NHeadersToHeaders(CFDictionaryRef dictRef)
        {
            CFDictionaryApplyFunction(dictRef, NHeaderToHeader, this);
        }
        static void NHeaderToHeader(const void* key, const void* value, void* ptr)
        {
            CHttpRequest* request = (CHttpRequest*)ptr;
            request->responseHeaders.Add((char*)key, (char*)value);
        }

        static void OnStateChanged(CHttpRequest* request, HttpRequestState status, CFReadStreamRef stream, CFStreamEventType event)
        {
            if (request->state>0) {
                request->state = status;
                HttpEventHandler* eh = request->client->GetEventHandler();
                if (eh!=0) eh->OnRequestStateChanged(request);                
            }
        }

        static void OnHeadersRecieved(CHttpRequest* request, CFReadStreamRef stream, CFStreamEventType event)
        {
            //headers
            CFDictionaryRef nHeaders = CFHTTPMessageCopyAllHeaderFields(request->cachedRequestMessage);
            request->NHeadersToHeaders(nHeaders);
            CFRelease(nHeaders);
            //responseStatus
            CFIndex code = CFHTTPMessageGetResponseStatusCode(request->cachedRequestMessage);
            request->responseStatus = (int)code;
            //responsePhrase
            CFStringRef phrase = CFHTTPMessageCopyResponseStatusLine(request->cachedRequestMessage);
            if (phrase!=0)
            {
                request->reasonPhrase = CFStringGetCStringPtr(phrase, kCFStringEncodingUTF8);
                CFRelease(phrase);
            } else {
                request->reasonPhrase = CHttpRequest::HttpCodeToPhrase((int)code);
            }
            //CFRelease(nHeaders); //{TODO} this crashes...why?
        }

        static void OnByteDataRecieved(CHttpRequest* request, CFReadStreamRef stream, CFStreamEventType event)
        {
            UInt8 buff[1024];
            CFIndex nBytesRead = CFReadStreamRead(stream, buff, 1024);
            
            if(nBytesRead>0)
            {
                CFWriteStreamWrite(request->cachedContentStream, (UInt8*)(&buff), (CFIndex)nBytesRead);
                request->readPosition += nBytesRead;
            }
            HttpEventHandler* eh = request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestProgress(request, request->readPosition, 0, false, HttpTransferDirection_DOWNLOAD);
        }

        static void OnTimeout(CHttpRequest* request, CFReadStreamRef stream, CFStreamEventType event)
        {
            HttpEventHandler* eh = request->client->GetEventHandler();
            if (eh!=0) eh->OnRequestTimeout(request);
        }

        static void OnError(CHttpRequest* request, CFReadStreamRef stream, CFStreamEventType event)
        {
            HttpEventHandler* eh = request->client->GetEventHandler();
            request->state = HttpRequestStateDone;
            request->errored = true;

            CFStreamError err = CFReadStreamGetError(stream);
            if (err.domain == kCFStreamErrorDomainPOSIX)
            {
                String error = String("Posix Error: ") + String((int)err.error);
                if (eh!=0) eh->OnRequestError(request);
            } else if (err.domain == kCFStreamErrorDomainMacOSStatus) {
                //OSStatus macError = (OSStatus)err.error; {TODO} use this
                String error = String("OSX Error: ") + String((int)err.error);
                if (eh!=0) eh->OnRequestError(request);
            } else if (err.domain == kCFStreamErrorDomainHTTP) {
                CFStreamErrorHTTP httpErr = (CFStreamErrorHTTP)err.error;
                String message;
                switch (httpErr) {
                case kCFStreamErrorHTTPParseFailure:
                    message = "CFStreamError: HTTP Parse Failure";
                    break;
                case kCFStreamErrorHTTPRedirectionLoop:
                    message = "CFStreamError: HTTP Redirection Loop";
                    break;
                case kCFStreamErrorHTTPBadURL:
                    message = "CFStreamError: HTTP Bad Url";
                            
                    break;
                default:
                    message = "Unknown Http Error: ";
                    message += String((int)err.error);
                    break;
                }
                if (eh!=0) eh->OnRequestError(request);
            } else {
                String error = String("Unidentified Error in XLI Http: ") + String((int)err.error);
                if (eh!=0) eh->OnRequestError(request);
            }
        }

        static void AsyncCallback(CFReadStreamRef stream, CFStreamEventType event, void* ptr)
        {
            CHttpRequest* request = (CHttpRequest*)ptr;
            switch (event)
            {
            case kCFStreamEventOpenCompleted:
                CHttpRequest::OnHeadersRecieved(request, stream, event);
                CHttpRequest::OnStateChanged(request, HttpRequestStateHeadersReceived, stream, event);
                request->StartDownload();
                break;
            case kCFStreamEventCanAcceptBytes:
                // {TODO} not currently used but will be needed for upload progress
                break;
            case kCFStreamEventHasBytesAvailable:
                if (request->reading == true)
                {
                    request->dataReady = true;
                    CHttpRequest::OnByteDataRecieved(request, stream, event);
                } else {
                    request->dataReady = true;
                }
                break;
            case kCFStreamEventErrorOccurred:
                CHttpRequest::OnError(request, stream, event);
                break;
            case kCFStreamEventEndEncountered:
                request->state = HttpRequestStateDone;
                request->getContentArray();
                CHttpRequest::OnStateChanged(request, HttpRequestStateDone, stream, event);

                if (request->cachedUploadData!=0) CFRelease(request->cachedUploadData);
                if (request->cachedRequestMessage!=0) CFRelease(request->cachedRequestMessage);
                if (request->cachedReadStream!=0)
                {
                    CFReadStreamClose(request->cachedReadStream);
                    CFRelease(request->cachedReadStream);
                }                
                break;
            }
        }
        
        static String HttpCodeToPhrase(int code)
        {
            switch (code)
            {
                case 100:
                    return "Continue";
                case 101:
                    return "Switching Protocols";
                case 200:
                    return "OK";
                case 201:
                    return "Created";
                case 202:
                    return "Accepted";
                case 203:
                    return "Non-Authoritative Information";
                case 204:
                    return "No Content";
                case 205:
                    return "Reset Content";
                case 206:
                    return "Partial Content";
                case 300:
                    return "Multiple Choices";
                case 301:
                    return "Moved Permanently";
                case 302:
                    return "Found";
                case 303:
                    return "See Other";
                case 304:
                    return "Not Modified";
                case 305:
                    return "Use Proxy";
                case 307:
                    return "Temporary Redirect";
                case 400:
                    return "Bad Request";
                case 401:
                    return "Unauthorized";
                case 402:
                    return "Payment Required";
                case 403:
                    return "Forbidden";
                case 404:
                    return "Not Found";
                case 405:
                    return "Method Not Allowed";
                case 406:
                    return "Not Acceptable";
                case 407:
                    return "Proxy Authentication Required";
                case 408:
                    return "Request Time-out";
                case 409:
                    return "Conflict";
                case 410:
                    return "Gone";
                case 411:
                    return "Length Required";
                case 412:
                    return "Precondition Failed";
                case 413:
                    return "Request Entity Too Large";
                case 414:
                    return "Request-URI Too Large";
                case 415:
                    return "Unsupported Media Type";
                case 416:
                    return "Requested range not satisfiable";
                case 417:
                    return "Expectation Failed";
                case 500:
                    return "Internal Server Error";
                case 501:
                    return "Not Implemented";
                case 502:
                    return "Bad Gateway";
                case 503:
                    return "Service Unavailable";
                case 504:
                    return "Gateway Time-out";
                case 505:
                    return "HTTP Version not supported";
                default:
                    return "INVALID";
            }
        }
    };

    class CHttpClient : public HttpClient
    {
    private:
        Managed<HttpEventHandler> eventHandler;
    public:
        virtual CHttpRequest* CreateRequest(const String& method, const String& url)
        {
            return new CHttpRequest(this, url, method);
        }
        virtual void Update() { }

        virtual void SetEventHandler(HttpEventHandler* eventHandler)
        {
            this->eventHandler = eventHandler;
        }

        virtual HttpEventHandler* GetEventHandler() 
        {
            return eventHandler;
        }
    };

    HttpClient* HttpClient::Create()
    {
        return new CHttpClient();
    }
};
