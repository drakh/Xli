#include "AInternal.h"
#include "AStream.h"
#include "AXliJ.h"
#include <XliHttp/HttpClient.h>
#include <Xli/HashMap.h>

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, Xli::GetAppName(), __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, Xli::GetAppName(), __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, Xli::GetAppName(), __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, Xli::GetAppName(), __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, Xli::GetAppName(), __VA_ARGS__))

namespace Xli
{
    class AHttpResponse: public HttpResponse
    {
    private:
        jobject httpConnection;
    public:
        AHttpResponse(Stream* payload, jobject connection) 
        {
            if (payload && connection)
            {
                this->Payload = payload;
                this->httpConnection = connection;
                this->valid = true;
            } else {
                this->valid = false;
            }
        }

        virtual int GetResponseCode() const
        {
            return XliJ::GetResponseCode(this->httpConnection);
        }
        
        AHttpResponse(bool valid)
        {
            this->valid = valid;
        }
        
        virtual ~AHttpResponse() 
        {
            if(this->valid)
            {
                Payload->Close();
                delete Payload;
                XliJ::HttpCloseConnection(this->httpConnection);
                JniHelper jni;
                jni->DeleteGlobalRef(httpConnection);
            }
        }

        virtual bool IsValid() const { return this->valid; }

        virtual String GetHeader(const String& key)
        {
            return XliJ::HttpGetHeader(httpConnection, key);
        }
    };

    //--------------------------------------------------

    class AHttpRequest: public HttpRequest
    {
    private:
        HashMap<String,String> headers;
        bool hasPayload;
        Stream* payload;
    public:

        AHttpRequest() 
        {
            this->hasPayload = false;
            this->SetMethod(HttpMethods::GET);
        }
        
        virtual ~AHttpRequest() 
        {
        }

        virtual void SetPayload(Stream* payload)
        {
            this->payload = payload;
            this->hasPayload = true;
        }

        virtual Stream* GetPayload() const
        {
            if (this->hasPayload)
            {
                return this->payload;
            } else {
                return new AStream();
            }
        }

        virtual bool HasPayload() const
        {
            return this->hasPayload;
        }

        virtual void SetHeader(const String& key, const String& val)
        {
            headers.Add(key, val);
        }

        virtual const HashMap<String,String>& GetHeaders() const
        {
            return this->headers;
        }
    };

	HttpRequest* HttpRequest::Create()
	{
		return new AHttpRequest();
	}

    //--------------------------------------------------

    class AHttpClient: public HttpClient
    {
    public:
        AHttpClient() {}

        virtual ~AHttpClient() {}

        virtual HttpResponse* Send(const String& uri, const HttpRequest& req)
        {
            if (!XliJ::ConnectedToNetwork())
            {
                LOGE("Not connected to network");
                return new AHttpResponse(false);
            }
            JniHelper jni;
            jobject connection = XliJ::HttpNewConnection(uri, req.GetMethodAsString(), 
                                                         req.HasPayload());
            if (!connection) {
                LOGE("Could not make connection to %s", uri.Data());
                return new AHttpResponse(false);
            }
            jni->NewGlobalRef(connection);
            const HashMap<String,String>& headers = req.GetHeaders();
            for (int i = 0; i < headers.Count(); ++i)
            {
                XliJ::HttpSetHeader(connection, headers.GetKey(i), 
                                    headers.GetValue(i));
            }
            // pump stream
            if (req.HasPayload())
            {
                Stream* payload = req.GetPayload();
                Managed<AStream> outStream = XliJ::HttpGetOutputStream(connection);
                char buffer[100];
                int bytesRead = 0;
                while (!payload->AtEnd()) 
                {
                    bytesRead = payload->Read(buffer,1,100);
                    outStream->Write(buffer, 1, bytesRead);
                }
            }
            // return a response
            //[TODO] set method of response
            Stream* result_payload = (Stream*)XliJ::HttpGetInputStream(connection);

            HttpResponse* response = new AHttpResponse(result_payload, connection);
            return response;
        }
    };


	void HttpClient::Init()
	{
        XliJ::InitDefaultCookieManager();
	}

	void HttpClient::Done()
	{
	}

	HttpClient* HttpClient::Create()
	{
		return new AHttpClient();
	}
};

