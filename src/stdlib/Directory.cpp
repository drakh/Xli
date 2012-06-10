#include <Xli/Directory.h>

namespace Xli
{
	Directory::Directory(const String& path)
	{
		this->path = path + "/";
	}

	Stream* Directory::OpenFile(const String& fileName, FileMode mode)
	{
		return new File(path+fileName, mode);
	}
}
