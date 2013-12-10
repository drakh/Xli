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

		virtual HttpResponse* Send(const String& uri, const HttpRequest& req)
		{
		}
	};

	void HttpClient::Init()
	{
	}

	void HttpClient::Done()
	{
	}

	HttpClient* HttpClient::Create()
	{
		return new DummyClient();
	}
}
