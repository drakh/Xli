#include <XliHttp/Uri.h>
#include <Xli/StringBuilder.h>
#include <cstdio>
#include <cctype>

namespace Xli
{
	String Uri::Encode(const String& uri)
	{
		StringBuilder sb;

		for (int i = 0; i < uri.Length(); i++)
		{
			if (isalnum(uri[i]))
			{
				sb.Append(uri[i]);
			}
			else if (uri[i] == '.' || uri[i] == '/' || uri[i] == ':')
			{
				sb.Append(uri[i]);
			}
			else
			{
				char buf[3];
				sprintf_s(buf, 3, "%2x", (unsigned int)uri[i]);
				sb.Append('%');
				sb.Append(buf[0]);
				sb.Append(buf[1]);
			}
		}

		return sb.GetString();
	}

	String Uri::Decode(const String& uri)
	{
		// TODO
		return uri;
	}
}
