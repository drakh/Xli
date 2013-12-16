#include <Xli/HashMap.h>
#include <Xli/PlatformSpecific/Android.h>
#include <XliHttp/HttpClient.h>


namespace Xli
{

    ///
    /// Wrapper for HttpClient/Request/Respons.
    /// This is used together with HttpImpl.cpp.uxl
    /// in the Experimental.Http.Net package
    ///
    void HttpClientWrapper::Open(String method, String url)
    {
        this->method = method;
        this->url = url;
        this->client = HttpClient::Create();
        this->request = HttpRequest::Create();
        this->request->SetMethod(Xli::HttpMethods::StringToMethod(method));
    }

    void HttpClientWrapper::Send(String data)
    {
        if (response != NULL) response = client->Send(url, (*request));
    }

    void HttpClientWrapper::SetRequestHeader(String header, String value)
    {
        if (request != NULL) request->SetHeader(header, value);
    }

    String HttpClientWrapper::GetResponseText()
    {
        // return this->response->Paload
        return "Not implemented";
    }

    unsigned short HttpClientWrapper::GetStatus()
    {
        if (response != NULL) return response->GetResponseCode();
        else return 0;
    }

    String HttpClientWrapper::GetResponseHeader(String header)
    {
        if (response != NULL) return response->GetHeader(header);
        else return "NULL";
    }

    String HttpClientWrapper::GetAllResponseHeaders()
    {
        return "Not implemented";
    }

    String HttpClientWrapper::GetStatusText()
    {
        return "Not implemented";
    }

    HttpClientWrapper * HttpClientWrapper::Create()
    {
        return Xli::Managed<HttpClientWrapper>(new HttpClientWrapper);
    }

    ///==================================================================\\\

    namespace PlatformSpecific
    {
        class AHttpResponse: public HttpResponse
        {
            jobject httpConnection;

        public:
            AHttpResponse(Stream* payload, jobject connection) 
            {
                if (payload && connection)
                {
                    this->Payload = payload;
                    this->httpConnection = connection;
                    this->valid = true;
                } 
                else 
                {
                    this->valid = false;
                }
            }

            virtual int GetResponseCode() const
            {
                return AShim::GetResponseCode(this->httpConnection);
            }
            
            AHttpResponse(bool valid)
            {
                this->valid = valid;
            }
            
            virtual ~AHttpResponse() 
            {
                if (this->valid)
                {
                    Payload->Close();
                    delete Payload;
                    AShim::HttpCloseConnection(this->httpConnection);
                    AJniHelper jni;
                    jni->DeleteGlobalRef(httpConnection);
                }
            }

            virtual bool IsValid() const { return this->valid; }

            virtual String GetHeader(const String& key)
            {
                return AShim::HttpGetHeader(httpConnection, key);
            }
        };

        //--------------------------------------------------

        class AHttpRequest: public HttpRequest
        {
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

        //--------------------------------------------------

        class AHttpClient: public HttpClient
        {
        public:
            AHttpClient() {}

            virtual ~AHttpClient() {}

            virtual HttpResponse* Send(const String& uri, const HttpRequest& req)
            {
                if (!AShim::ConnectedToNetwork())
                {
                    LOGE("Not connected to network");
                    return new AHttpResponse(false);
                }
                AJniHelper jni;
                jobject connection = AShim::HttpNewConnection(uri, req.GetMethodAsString(), 
                                                             req.HasPayload());
                if (!connection) {
                    LOGE("Could not make connection to %s", uri.Data());
                    return new AHttpResponse(false);
                }
                jni->NewGlobalRef(connection);
                const HashMap<String,String>& headers = req.GetHeaders();
                for (int i = 0; i < headers.Count(); ++i)
                {
                    AShim::HttpSetHeader(connection, headers.GetKey(i), 
                                        headers.GetValue(i));
                }
                // pump stream
                if (req.HasPayload())
                {
                    Stream* payload = req.GetPayload();
                    Managed<AStream> outStream = AShim::HttpGetOutputStream(connection);
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
                Stream* result_payload = (Stream*)AShim::HttpGetInputStream(connection);

                HttpResponse* response = new AHttpResponse(result_payload, connection);
                return response;
            }
        };
    }

    HttpRequest* HttpRequest::Create()
    {
        return new PlatformSpecific::AHttpRequest();
    }

	void HttpClient::Init()
	{
        PlatformSpecific::AShim::InitDefaultCookieManager();
	}

	void HttpClient::Done()
	{
	}

	HttpClient* HttpClient::Create()
	{
		return new PlatformSpecific::AHttpClient();
	}



};

