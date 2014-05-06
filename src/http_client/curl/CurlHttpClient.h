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
};
