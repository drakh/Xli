#ifndef __XLI_MEDIA_GZIP_H__
#define __XLI_MEDIA_GZIP_H__

#include <Xli/Stream.h>

namespace Xli
{
	/**
		\ingroup XliMediaCompression
	*/
	class GZip
	{
		GZip();

	public:
		static Stream* CreateReader(Stream* sourceStream);
		static Stream* CreateWriter(Stream* targetStream, int level = 5, bool header = true);
	};
}

#endif
