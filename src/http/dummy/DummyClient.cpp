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
