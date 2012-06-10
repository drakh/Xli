#ifndef __XLI_DIRECTORY_H__
#define __XLI_DIRECTORY_H__

#include <Xli/File.h>

namespace Xli
{
	class Directory: public FileSystem
	{
		String path;

	public:
		Directory(const String& path);
		virtual ~Directory() {}

		virtual Stream* OpenFile(const String& fileName, FileMode mode);
	};
}

#endif
