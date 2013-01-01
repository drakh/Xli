#ifndef __XLI_ZIP_H__
#define __XLI_ZIP_H__

#include <Xli/Object.h>
#include <Xli/FileSystem.h>
#include <Xli/File.h>

namespace Xli
{
	/**
		\ingroup Compression
	*/
	class Zip
	{
	public:
		static FileSystem* Open(Stream* file, const char* password);
		static inline FileSystem* Open(Stream* file) { return Open(file, 0); }
		static inline FileSystem* Open(Stream* file, const CharString& password) { return Open(file, password.Data()); }
		static inline FileSystem* Open(const String& fileName) { Managed<File> f = new File(fileName, FileModeRead); return Open(f); }
		static inline FileSystem* Open(const String& fileName, const CharString& password) { Managed<File> f = new File(fileName, FileModeRead); return Open(f, password.Data()); }
	};
}

#endif
