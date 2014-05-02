//{TODO} IFDEFS HERE
#include <XliHttpClient/HttpClient.h>
#include <Xli/Window.h>
#include <curl/curl.h>

namespace Xli
{
    class CurlHttpRequest;

    class CurlHttpClient : public HttpClient
    {
    private:
        Managed<HttpEventHandler> eventHandler;
        HashMap<CURL*, CurlHttpRequest*> sessionTable;
        int keyCounter;
        CURLM* multiSession;
    public:
        static HttpClient* Create();
        CurlHttpClient();
        virtual ~CurlHttpClient();
        virtual HttpRequest* CreateRequest(const String& method, const String& url);
        virtual void SetEventHandler(HttpEventHandler* eventHandler);
        virtual void AddSession(CURL* session, CurlHttpRequest* request);
        virtual void RemoveSession(CURL* session);
        virtual HttpEventHandler* GetEventHandler();
        virtual void Update();
    };


    class SessionMap
    {
    private:
        static HashMap<HttpRequest*, CURL*> abortedTable;
    public:
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

    inline CURLoption methodToCurlOption(String method)
    {
        if (method == "GET") return CURLOPT_HTTPGET;
        if (method == "POST") return CURLOPT_POST;
        if (method == "OPTIONS") XLI_THROW("MethodToCurlCode: Unimplemented Method Code");
        if (method == "HEAD") XLI_THROW("MethodToCurlCode: Unimplemented Method Code");
        if (method == "PUT") return CURLOPT_UPLOAD;
        if (method == "DELETE") XLI_THROW("MethodToCurlCode: Unimplemented Method Code");
        if (method == "TRACE") XLI_THROW("MethodToCurlCode: Unimplemented Method Code");
        XLI_THROW("MethodToCurlCode: Invalid Method Code");
    }

    inline String CurlErrorToString(CURLcode errorCode)
    {
        switch(errorCode)
        {
        case CURLE_OK: return String("OK");
        case CURLE_UNSUPPORTED_PROTOCOL: return String("UNSUPPORTED_PROTOCOL");
        case CURLE_FAILED_INIT: return String("FAILED_INIT");
        case CURLE_URL_MALFORMAT: return String("URL_MALFORMAT");
        case CURLE_NOT_BUILT_IN: return String("NOT_BUILT_IN");
        case CURLE_COULDNT_RESOLVE_PROXY: return String("COULDNT_RESOLVE_PROXY");
        case CURLE_COULDNT_RESOLVE_HOST: return String("COULDNT_RESOLVE_HOST");
        case CURLE_COULDNT_CONNECT: return String("COULDNT_CONNECT:");
        case CURLE_FTP_WEIRD_SERVER_REPLY: return String("FTP_WEIRD_SERVER_REPLY");
        case CURLE_REMOTE_ACCESS_DENIED: return String("REMOTE_ACCESS_DENIED");
        //case CURLE_FTP_ACCEPT_FAILED: return String("FTP_ACCEPT_FAILED");
        case CURLE_FTP_WEIRD_PASS_REPLY: return String("FTP_WEIRD_PASS_REPLY");
        //case CURLE_FTP_ACCEPT_TIMEOUT: return String("FTP_ACCEPT_TIMEOUT");
        case CURLE_FTP_WEIRD_PASV_REPLY: return String("FTP_WEIRD_PASV_REPLY");
        case CURLE_FTP_WEIRD_227_FORMAT: return String("FTP_WEIRD_227_FORMAT");
        case CURLE_FTP_CANT_GET_HOST: return String("FTP_CANT_GET_HOST");
        case CURLE_FTP_COULDNT_SET_TYPE: return String("FTP_COULDNT_SET_TYPE");
        case CURLE_PARTIAL_FILE: return String("PARTIAL_FILE");
        case CURLE_FTP_COULDNT_RETR_FILE: return String("FTP_COULDNT_RETR_FILE");
        case CURLE_QUOTE_ERROR: return String("QUOTE_ERRO");
        case CURLE_HTTP_RETURNED_ERROR: return String("HTTP_RETURNED_ERROR");
        case CURLE_WRITE_ERROR: return String("WRITE_ERROR");
        case CURLE_UPLOAD_FAILED: return String("UPLOAD_FAILED");
        case CURLE_READ_ERROR: return String("READ_ERROR");
        case CURLE_OUT_OF_MEMORY: return String("OUT_OF_MEMORY:");
        case CURLE_OPERATION_TIMEDOUT: return String("OPERATION_TIMEDOUT");
        case CURLE_FTP_PORT_FAILED: return String("FTP_PORT_FAILED");
        case CURLE_FTP_COULDNT_USE_REST: return String("FTP_COULDNT_USE_RES");
        case CURLE_RANGE_ERROR: return String("RANGE_ERROR");
        case CURLE_HTTP_POST_ERROR: return String("HTTP_POST_ERROR");
        case CURLE_SSL_CONNECT_ERROR: return String("SSL_CONNECT_ERROR");
        case CURLE_BAD_DOWNLOAD_RESUME: return String("BAD_DOWNLOAD_RESUME");
        case CURLE_FILE_COULDNT_READ_FILE: return String("FILE_COULDNT_READ_FILE");
        case CURLE_LDAP_CANNOT_BIND: return String("LDAP_CANNOT_BIND");
        case CURLE_LDAP_SEARCH_FAILED: return String("LDAP_SEARCH_FAILED");
        case CURLE_FUNCTION_NOT_FOUND: return String("FUNCTION_NOT_FOUND");
        case CURLE_ABORTED_BY_CALLBACK: return String("ABORTED_BY_CALLBACK");
        case CURLE_BAD_FUNCTION_ARGUMENT: return String("BAD_FUNCTION_ARGUMENT");
        case CURLE_INTERFACE_FAILED: return String("INTERFACE_FAILED");
        case CURLE_TOO_MANY_REDIRECTS: return String("TOO_MANY_REDIRECTS");
        case CURLE_UNKNOWN_OPTION: return String("UNKNOWN_OPTION");
        case CURLE_TELNET_OPTION_SYNTAX: return String("TELNET_OPTION_SYNTAX");
        case CURLE_PEER_FAILED_VERIFICATION: return String("PEER_FAILED_VERIFICATION");
        case CURLE_GOT_NOTHING: return String("GOT_NOTHING");
        case CURLE_SSL_ENGINE_NOTFOUND: return String("SSL_ENGINE_NOTFOUND");
        case CURLE_SSL_ENGINE_SETFAILED: return String("SSL_ENGINE_SETFAILED");
        case CURLE_SEND_ERROR: return String("SEND_ERROR");
        case CURLE_RECV_ERROR: return String("RECV_ERROR");
        case CURLE_SSL_CERTPROBLEM: return String("SSL_CERTPROBLEM");
        case CURLE_SSL_CIPHER: return String("SSL_CIPHER");
        case CURLE_SSL_CACERT: return String("SSL_CACERT");
        case CURLE_BAD_CONTENT_ENCODING: return String("BAD_CONTENT_ENCODING");
        case CURLE_LDAP_INVALID_URL: return String("LDAP_INVALID_URL");
        case CURLE_FILESIZE_EXCEEDED: return String("FILESIZE_EXCEEDED");
        case CURLE_USE_SSL_FAILED: return String("USE_SSL_FAILED");
        case CURLE_SEND_FAIL_REWIND: return String("SEND_FAIL_REWIND");
        case CURLE_SSL_ENGINE_INITFAILED: return String("SSL_ENGINE_INITFAILED");
        case CURLE_LOGIN_DENIED: return String("LOGIN_DENIE");
        case CURLE_TFTP_NOTFOUND: return String("TFTP_NOTFOUND");
        case CURLE_TFTP_PERM: return String("TFTP_PERM");
        case CURLE_REMOTE_DISK_FULL: return String("REMOTE_DISK_FULL");
        case CURLE_TFTP_ILLEGAL: return String("TFTP_ILLEGAL");
        case CURLE_TFTP_UNKNOWNID: return String("TFTP_UNKNOWNID");
        case CURLE_REMOTE_FILE_EXISTS: return String("REMOTE_FILE_EXISTS");
        case CURLE_TFTP_NOSUCHUSER: return String("TFTP_NOSUCHUSER");
        case CURLE_CONV_FAILED: return String("CONV_FAILED");
        case CURLE_CONV_REQD: return String("CONV_REQD");
        case CURLE_SSL_CACERT_BADFILE: return String("SSL_CACERT_BADFILE");
        case CURLE_REMOTE_FILE_NOT_FOUND: return String("REMOTE_FILE_NOT_FOUND");
        case CURLE_SSH: return String("SSH");
        case CURLE_SSL_SHUTDOWN_FAILED: return String("SSL_SHUTDOWN_FAILED");
        case CURLE_AGAIN: return String("AGAIN");
        case CURLE_SSL_CRL_BADFILE: return String("SSL_CRL_BADFILE");
        case CURLE_SSL_ISSUER_ERROR: return String("SSL_ISSUER_ERROR");
        case CURLE_FTP_PRET_FAILED: return String("FTP_PRET_FAILED");
        case CURLE_RTSP_CSEQ_ERROR: return String("RTSP_CSEQ_ERROR");
        case CURLE_RTSP_SESSION_ERROR: return String("RTSP_SESSION_ERROR");
        case CURLE_FTP_BAD_FILE_LIST: return String("FTP_BAD_FILE_LIST");
        case CURLE_CHUNK_FAILED: return String("CHUNK_FAILED");
        //case CURLE_NO_CONNECTION_AVAILABLE: return String("NO_CONNECTION_AVAILABLE");
        default: return String("-INVALID CURL ERROR CODE-");
        }
    }
};
