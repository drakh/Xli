#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#include <Xli/PlatformSpecific/iOS.h>
#include <XliHttp/HttpClient.h>
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
        Managed< HttpStateChangedHandler > stateChangedCallback;
        Managed< HttpProgressHandler > progressCallback;
        Managed< HttpTimeoutHandler > timeoutCallback;
        Managed< HttpErrorHandler > errorCallback;
        HashMap<String,String> headers;
        HashMap<String,String> responseHeaders;
        HttpRequestState status;
        HttpMethodType method;
        int responseStatus;
        String reasonPhrase;
        String url;
        int timeout;

        CFHTTPMessageRef cachedRequestMessage;
        CFReadStreamRef cachedReadStream;
        CFDataRef cachedUploadData;
        bool errored;
        bool headersSet;
        
        bool dataReady;
        bool reading;
        bool contentAsString; // {TODO} use this
        int readPosition;
        String cachedContentString;
        CFWriteStreamRef cachedContentStream;
    public:

        CHttpRequest() 
        {
            this->status = HttpUnsent;
            this->url = "";
            this->method = HttpGetMethod;
            this->timeout = 0;
            this->readPosition = 0;
            this->reading = false;
            this->cachedContentStream = 0;
            this->cachedReadStream = 0;
            this->cachedRequestMessage = 0;
        }

        CHttpRequest(String url, HttpMethodType method) 
        {
            this->status = HttpUnsent;
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
            this->status = HttpDone;
            
            if (cachedRequestMessage != 0)
                CFRelease(this->cachedRequestMessage);
            if (cachedReadStream!=0)
            {
                CFReadStreamClose(this->cachedReadStream);
                CFRelease(this->cachedReadStream);
            }
            if (cachedReadStream!=0)
                CFRelease(this->cachedReadStream);
            if (cachedContentStream!=0)
                CFRelease(this->cachedContentStream);
            if (cachedUploadData!=0)
                CFRelease(this->cachedUploadData);
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
            if (this->status == HttpHeadersReceived) // {TODO} is this the correct state?
            {
                return this->responseStatus;
            } else {
                XLI_THROW("HttpRequest->GetResponseStatus(): Not in a valid state to get the response status");
            }
        }

        virtual String GetReasonPhrase() const
        {
            if (this->status == HttpHeadersReceived) // {TODO} is this the correct state?
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
        virtual String GetResponseHeaderKeyN(int n) const
        {
            if (this->status == HttpHeadersReceived)
            {
                return this->responseHeaders.GetKey(n);
            } else {
                XLI_THROW("HttpRequest->GetResponseHeaderN(): Not in a valid state to get the response header");
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

        virtual void Send(void* content, long byteLength)
        {
            if (this->status != HttpUnsent) return;
            
            this->contentAsString = false;
            
            CFStringRef nUrlStr = CFStringCreateWithCString(kCFAllocatorDefault, this->url.DataPtr(), kCFStringEncodingUTF8);
            CFURLRef nUrl = CFURLCreateWithString(kCFAllocatorDefault, nUrlStr, NULL);
            CFStringRef nMethod = CFStringCreateWithCString(kCFAllocatorDefault, HttpMethodToString(this->method).DataPtr(), kCFStringEncodingUTF8);
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
            
            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->stateChangedCallback->OnResponse(this, this->status);
            
            CFRelease(nUrlStr);
            CFRelease(nMethod);
            CFRelease(nUrl);
        }

        virtual void Send(String content)
        {
            if (this->status != HttpUnsent) return;
            
            this->cachedContentString = "";
            this->contentAsString = true;
            
            CFStringRef nUrlStr = CFStringCreateWithCString(kCFAllocatorDefault, this->url.DataPtr(), kCFStringEncodingUTF8);
            CFURLRef nUrl = CFURLCreateWithString(kCFAllocatorDefault, nUrlStr, NULL);
            CFStringRef nMethod = CFStringCreateWithCString(kCFAllocatorDefault, HttpMethodToString(this->method).DataPtr(), kCFStringEncodingUTF8);
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
            
            this->status = HttpSent;
            if (this->stateChangedCallback!=0) this->stateChangedCallback->OnResponse(this, this->status);
            
            CFRelease(nUrlStr);
            CFRelease(nMethod);
            CFRelease(nUrl);
        }

        virtual void Send()
        {
            this->Send("");
        }

        virtual void SetCFHeaders(CFHTTPMessageRef message)
        {
            if (!this->headersSet && this->status == HttpUnsent)
            {
                this->headersSet = true;
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
            if (this->status > 1 && this->status < 5 )
            this->status = HttpDone; //{TODO} how does statechanged know if it was successful?
            
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
            
            if (this->stateChangedCallback!=0) this->stateChangedCallback->OnResponse(this, this->status);
        }

        virtual void PullContentString()
        {
            if (this->status==HttpHeadersReceived)
            {
                this->status = HttpLoading;
                reading=true;
                this->contentAsString = true;
                if (this->stateChangedCallback!=0) this->stateChangedCallback->OnResponse(this, this->status);
                if (dataReady) OnStringProgress(this, cachedReadStream, NULL);
            } else {
                NSLog(@"Cant pull content string"); //{TODO} proper error here
            }
        }
        virtual String GetContentString()
        {
            if (this->status == HttpDone)
                return this->cachedContentString;
            NSLog(@"Cant get content string");
            return ""; // {TODO} throw error?
        }        

        virtual void PullContentArray() // {TODO} this needs to be a byte array async
        {    
            if ((this->status==HttpHeadersReceived) && (this->cachedContentStream == 0))
            {
                this->status = HttpLoading;
                this->contentAsString = false;
                this->reading = true;
                if (this->stateChangedCallback!=0) this->stateChangedCallback->OnResponse(this, this->status);
          
                this->cachedContentStream = CFWriteStreamCreateWithAllocatedBuffers(kCFAllocatorDefault, kCFAllocatorDefault);
                CFWriteStreamOpen(this->cachedContentStream);
                if (dataReady) OnByteProgress(this, cachedReadStream, NULL);
            } else {
                NSLog(@"Cant pull content array"); //{TODO} proper error here
            }
        }
        virtual void* GetContentArray() // {TODO} this needs to be a byte array async
        {            
            if (this->status == HttpDone && !this->errored && !this->contentAsString)
            {
                CFStringRef prop = CFSTR("kCFStreamPropertyDataWritten");
                CFDataRef streamDataHandle = (CFDataRef)CFWriteStreamCopyProperty(this->cachedContentStream, prop);
                long len = CFDataGetLength(streamDataHandle);
                this->readPosition = len;
                
                void* data = malloc(len);
                CFDataGetBytes(streamDataHandle, CFRangeMake(0,len), (UInt8*)data);

                CFWriteStreamClose(this->cachedContentStream);
                CFRelease(this->cachedContentStream);
                CFRelease(streamDataHandle);//{TODO} test this
                this->cachedContentStream = 0;
                return data;
            }
            NSLog(@"Cant get content array"); // {TODO} error needed here
            return 0;
        }
        virtual long GetContentArrayLength()
        {
            if ((this->status == HttpDone) && (!this->errored) && (!this->contentAsString))
            {
                return this->readPosition;
            }
            NSLog(@"Cant get content array length");
            return -1; // {TODO} throw error?
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
            if (request->status>0) {
                request->status = status;
                if (request->stateChangedCallback!=0)
                    request->stateChangedCallback->OnResponse(request, request->status);
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

        static void OnStringProgress(CHttpRequest* request, CFReadStreamRef stream, CFStreamEventType event)
        {
            UInt8 buff[1024];
            CFIndex nBytesRead = CFReadStreamRead(stream, buff, 1024);
            
            if( nBytesRead>0 )
            {
                request->cachedContentString.Append((char*)buff, (int)nBytesRead);
                request->readPosition+=nBytesRead;
            }
            if (request->progressCallback!=0)
                request->progressCallback->OnResponse(request, request->readPosition, 0, false);
        }
        
        static void OnByteProgress(CHttpRequest* request, CFReadStreamRef stream, CFStreamEventType event)
        {
            UInt8 buff[1024];
            CFIndex nBytesRead = CFReadStreamRead(stream, buff, 1024);
            
            if(nBytesRead>0)
            {
                CFWriteStreamWrite(request->cachedContentStream, (UInt8*)(&buff), (CFIndex)nBytesRead);
                request->readPosition += nBytesRead;
            }
            if (request->progressCallback!=0)
                request->progressCallback->OnResponse(request, request->readPosition, 0, false);
        }

        static void OnTimeout(CHttpRequest* request, CFReadStreamRef stream, CFStreamEventType event)
        {
            if (request->timeoutCallback!=0)
                request->timeoutCallback->OnResponse(request);                
        }

        static void OnError(CHttpRequest* request, CFReadStreamRef stream, CFStreamEventType event)
        {
            //HttpRequestState oldStatus = request->status; //{TODO} add this to android
            request->status = HttpDone;
            request->errored = true;
            if (request->errorCallback!=0)
            {
                CFStreamError err = CFReadStreamGetError(stream);
                if (err.domain == kCFStreamErrorDomainPOSIX)
                {
                    String error = String("Posix Error: ") + String((int)err.error);
                    request->errorCallback->OnResponse(request, err.error, error);
                } else if (err.domain == kCFStreamErrorDomainMacOSStatus) {
                    //OSStatus macError = (OSStatus)err.error; {TODO} use this
                    String error = String("OSX Error: ") + String((int)err.error);
                    request->errorCallback->OnResponse(request, err.error, error);
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
                    request->errorCallback->OnResponse(request, err.error, message);
                } else {
                    String error = String("Unidentified Error in XLI Http: ") + String((int)err.error);
                    request->errorCallback->OnResponse(request, err.error, error);
                }
            }
        }

        static void AsyncCallback(CFReadStreamRef stream, CFStreamEventType event, void* ptr)
        {
            CHttpRequest* request = (CHttpRequest*)ptr;
            switch (event)
            {
            case kCFStreamEventOpenCompleted:
                CHttpRequest::OnHeadersRecieved(request, stream, event);
                CHttpRequest::OnStateChanged(request, HttpHeadersReceived, stream, event);
                break;
            case kCFStreamEventCanAcceptBytes:
                // not currently used but will be needed for upload progress
                break;
            case kCFStreamEventHasBytesAvailable:
                if (request->reading == true)
                {
                    request->dataReady = true;
                    if (request->contentAsString) {
                        CHttpRequest::OnStringProgress(request, stream, event);
                    } else {
                        CHttpRequest::OnByteProgress(request, stream, event);
                    }
                    
                } else {
                    request->dataReady = true;
                }
                break;
            case kCFStreamEventErrorOccurred:
                CHttpRequest::OnError(request, stream, event);
                break;
            case kCFStreamEventEndEncountered:
                request->status = HttpDone;
                CHttpRequest::OnStateChanged(request, HttpDone, stream, event);
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
    public:
        CHttpClient() {}
        virtual ~CHttpClient() {}
        virtual HttpRequest* NewRequest()
        {
            return new CHttpRequest();
        }
    };
    
    HttpClient* HttpClient::Create()
    {
        return new CHttpClient();
    }
}
