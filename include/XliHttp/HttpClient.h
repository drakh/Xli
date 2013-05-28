#ifndef __XLI_HTTP_HTTP_CLIENT_H__
#define __XLI_HTTP_HTTP_CLIENT_H__

#include <Xli/String.h>
#include <Xli/Stream.h>

namespace Xli
{
	/**
		\ingroup Http
	*/
	class HttpResponse
	{
	public:
		Managed<Stream> HeaderStream;
		Managed<Stream> BodyStream;
	};

	/**
		\ingroup Http
	*/
	class HttpRequest
	{
	public:

	};

	/**
		\ingroup Http
	*/
	class HttpClient: public Object
	{
	public:
		static void Init();
		static void Done();

		static HttpClient* Create();

		virtual void Send(const String& uri, const HttpRequest& req, HttpResponse& res) = 0;
	};
}

#endif
