#include <XliHttp/HttpClient.h>

namespace Xli
{
	class DummyClient: public HttpClient
	{
	public:
		DummyClient()
		{
		}

		virtual ~DummyClient()
		{
		}

		virtual void Send(const String& uri, const HttpRequest& req, HttpResponse& res)
		{
		}
	};

	void HttpClient::Init()
	{
	}

	void HttpClient::Shutdown()
	{
	}

	HttpClient* HttpClient::Create()
	{
		return new DummyClient();
	}
}
