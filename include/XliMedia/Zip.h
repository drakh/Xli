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
		static FileSystem* Open(Stream* file, const char* password);

	public:
		static inline FileSystem* Open(Stream* file) 
		{ 
			return Open(file, 0); 
		}
		
		static inline FileSystem* Open(Stream* file, const String& password) 
		{ 
			return Open(file, password.Data()); 
		}
		
		static inline FileSystem* Open(const String& filename) 
		{ 
			Managed<File> f = new File(filename, FileModeRead); 
			return Open(f); 
		}
		
		static inline FileSystem* Open(const String& filename, const String& password) 
		{ 
			Managed<File> f = new File(filename, FileModeRead); 
			return Open(f, password.Data()); 
		}
	};
}

#endif
