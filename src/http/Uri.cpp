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
			int c = (int)(unsigned char)uri[i];

			if (isalnum(c) || c == '.' || c == '/' || c == ':')
			{
				sb.Append(uri[i]);
			}
			else
			{
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
