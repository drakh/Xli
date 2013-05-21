#ifndef __XLI_HTTP_HTTP_CLIENT_H__
#define __XLI_HTTP_HTTP_CLIENT_H__

#include <Xli/String.h>
#include <Xli/Stream.h>

namespace Xli
{
	class HttpResponse
	{
	public:
		Managed<Stream> HeaderStream;
		Managed<Stream> BodyStream;
	};

	class HttpClient: public Object
	{
	public:
		static void Init();
		static void Shutdown();

		static HttpClient* Create();


	};
}

#endif
