#import <CoreFoundation/CoreFoundation.h>
#import <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#include <Xli/PlatformSpecific/iOS.h>
#include <XliHttp/HttpClient.h>
#include <Xli/HashMap.h>
//-------------------------
#include <CFNetwork/CFNetwork.h>
#include <CFNetwork/CFHTTPStream.h>
#include <iostream>
#include <fstream>

namespace Xli
{

    //--------------------------------------------------

    class CHttpResponse: public HttpResponse
    {
    public:
        jobject body;
        CHttpResponse() {}
        virtual ~CHttpResponse() {}
        virtual String GetContentString()
        {
        }
        virtual Stream* GetContentStream()
        {
        }
    };
    HttpResponse* HttpResponse::Create()
    {
        return new CHttpResponse();
    }

    //--------------------------------------------------

    class CHttpRequest : public HttpRequest
    {
    public:
        CHttpRequest(String url, HttpMethodType method)
        {
            this->Url = url;
            this->Method = method;
            this->Timeout = 0;
            this->Callback = callback;
            this->Body = 0;
            this->BodySizeBytes = 0;
        }

        virtual ~CHttpRequest() 
        {
        }

        virtual void Send()
        {            
        }
        
        virtual void Abort()
        {
        }
    };
    HttpRequest* HttpRequest::Create(String url, HttpMethodType method,
                                     HttpResponseHandler* callback, const HttpClient* client)
    {
        return new CHttpRequest(url, method, callback);
    }
    
    //--------------------------------------------------

    class CHttpClient : public HttpClient
    {
    public:
        CHttpClient() {}
        virtual ~CHttpClient() {}
    };
    HttpClient* HttpClient::Create()
    {
        return new CHttpClient();
    }
}
