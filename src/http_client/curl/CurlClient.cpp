#include <XliHttpClient/HttpClient.h>
#include <curl/curl.h>

namespace Xli
{
    static String CURLcodeToString(CURLcode code)
    {
        switch (code)
        {
#define CASE(x) case x: return #x;
            CASE(CURLE_UNSUPPORTED_PROTOCOL);
            CASE(CURLE_FAILED_INIT);
            CASE(CURLE_URL_MALFORMAT);
            CASE(CURLE_COULDNT_RESOLVE_PROXY);
            CASE(CURLE_COULDNT_RESOLVE_HOST);
            CASE(CURLE_COULDNT_CONNECT);
            CASE(CURLE_FTP_WEIRD_SERVER_REPLY);
            CASE(CURLE_REMOTE_ACCESS_DENIED);
            CASE(CURLE_OBSOLETE10);
            CASE(CURLE_FTP_WEIRD_PASS_REPLY);
            CASE(CURLE_OBSOLETE12);
            CASE(CURLE_FTP_WEIRD_PASV_REPLY);
            CASE(CURLE_FTP_WEIRD_227_FORMAT);
            CASE(CURLE_FTP_CANT_GET_HOST);
            CASE(CURLE_OBSOLETE16);
            CASE(CURLE_FTP_COULDNT_SET_TYPE);
            CASE(CURLE_PARTIAL_FILE);
            CASE(CURLE_FTP_COULDNT_RETR_FILE);
            CASE(CURLE_OBSOLETE20);
            CASE(CURLE_QUOTE_ERROR);
            CASE(CURLE_HTTP_RETURNED_ERROR);
            CASE(CURLE_WRITE_ERROR);
            CASE(CURLE_UPLOAD_FAILED);
            CASE(CURLE_READ_ERROR);
            CASE(CURLE_OUT_OF_MEMORY);
            CASE(CURLE_OPERATION_TIMEDOUT);
            CASE(CURLE_FTP_PORT_FAILED);
            CASE(CURLE_FTP_COULDNT_USE_REST);
            CASE(CURLE_RANGE_ERROR);
            CASE(CURLE_HTTP_POST_ERROR);
            CASE(CURLE_SSL_CONNECT_ERROR);
            CASE(CURLE_BAD_DOWNLOAD_RESUME);
            CASE(CURLE_FILE_COULDNT_READ_FILE);
            CASE(CURLE_LDAP_CANNOT_BIND);
            CASE(CURLE_LDAP_SEARCH_FAILED);
            CASE(CURLE_FUNCTION_NOT_FOUND);
            CASE(CURLE_ABORTED_BY_CALLBACK);
            CASE(CURLE_BAD_FUNCTION_ARGUMENT);
            CASE(CURLE_INTERFACE_FAILED);
            CASE(CURLE_TOO_MANY_REDIRECTS);
            CASE(CURLE_UNKNOWN_TELNET_OPTION);
            CASE(CURLE_TELNET_OPTION_SYNTAX);
            CASE(CURLE_PEER_FAILED_VERIFICATION);
            CASE(CURLE_GOT_NOTHING);
            CASE(CURLE_SSL_ENGINE_NOTFOUND);
            CASE(CURLE_SSL_ENGINE_SETFAILED);
            CASE(CURLE_SEND_ERROR);
            CASE(CURLE_RECV_ERROR);
            CASE(CURLE_SSL_CERTPROBLEM);
            CASE(CURLE_SSL_CIPHER);
            CASE(CURLE_SSL_CACERT);
            CASE(CURLE_BAD_CONTENT_ENCODING);
            CASE(CURLE_LDAP_INVALID_URL);
            CASE(CURLE_FILESIZE_EXCEEDED);
            CASE(CURLE_USE_SSL_FAILED);
            CASE(CURLE_SEND_FAIL_REWIND);
            CASE(CURLE_SSL_ENGINE_INITFAILED);
            CASE(CURLE_LOGIN_DENIED);
            CASE(CURLE_TFTP_NOTFOUND);
            CASE(CURLE_TFTP_PERM);
            CASE(CURLE_REMOTE_DISK_FULL);
            CASE(CURLE_TFTP_ILLEGAL);
            CASE(CURLE_TFTP_UNKNOWNID);
            CASE(CURLE_REMOTE_FILE_EXISTS);
            CASE(CURLE_TFTP_NOSUCHUSER);
            CASE(CURLE_CONV_FAILED);
            CASE(CURLE_CONV_REQD);
            CASE(CURLE_SSL_CACERT_BADFILE);
            CASE(CURLE_REMOTE_FILE_NOT_FOUND);
            CASE(CURLE_SSH);
            CASE(CURLE_SSL_SHUTDOWN_FAILED);
            CASE(CURLE_AGAIN);
            CASE(CURLE_SSL_CRL_BADFILE);
            CASE(CURLE_SSL_ISSUER_ERROR);
#undef CASE
        }

        return "<Unknown CURLcode: " + (String)(int)code + ">";
    }

    class CurlClient : public HttpClient
    {
    public:
        CurlClient()
        {

        }

        virtual ~CurlClient()
        {

        }

        //virtual HttpResponse* Send(const String& uri, const HttpRequest& req) = 0;
        virtual HttpResponse* Send(const String& uri, const HttpRequest& req)
        {
            CURL* curl = curl_easy_init();
            if (!curl)
                XLI_THROW("CURL ERROR: Failed to create handle");

            CURLcode result = CURLE_OK;
            if (result == CURLE_OK) result = curl_easy_setopt(curl, CURLOPT_URL, uri.DataPtr());
            if (result == CURLE_OK) result = curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
            if (result == CURLE_OK) result = curl_easy_perform(curl);

            curl_easy_cleanup(curl);

            if (result != CURLE_OK)
                XLI_THROW("CURL ERROR: Request failed with code " + CURLcodeToString(result));
            return NULL;
        }
    };

    // TODO: RefCount
    static bool inited = false;

    void HttpClient::Init()
    {
        if (!inited)
        {
            if (curl_global_init(CURL_GLOBAL_ALL) != 0)
                XLI_THROW("CURL ERROR: Global init failed");

            inited = true;
        }
    }

    void HttpClient::Done()
    {
        if (inited)
        {
            curl_global_cleanup();
            inited = false;
        }
    }

    HttpClient* HttpClient::Create()
    {
        if (!inited) Init();
        return new CurlClient();
    }
    

#if defined(XLI_PLATFORM_WIN32)
    ///
    ///    Wrapper for HttpClient/Request/Respons.
    /// This is used together with HttpImpl.cpp.uxl
    /// in the Experimental.Http.Net package
    ///
    void HttpClientWrapper::Open(String method, String url)
    {
        this->method = method;
        this->url = url;
        this->client = HttpClient::Create();
        this->request = HttpRequest::Create();
        this->request->SetMethod(Xli::HttpStringToMethod(method));
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
#endif
}
