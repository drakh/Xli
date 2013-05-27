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
			if (uri[i] >= 32 && (isalnum(uri[i]) || uri[i] == '.' || uri[i] == '/' || uri[i] == ':'))
			{
				sb.Append(uri[i]);
			}
			else
			{
				unsigned int c = (unsigned int)(unsigned char)uri[i];
				static const char* hex = "0123456789ABCDEF";

				sb.Append('%');
				sb.Append(hex[(c >> 4) & 0xf]);
				sb.Append(hex[(c >> 0) & 0xf]);
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
