#ifndef __XLI_PATH_H__
#define __XLI_PATH_H__

#include "String.h"

namespace Xli
{
	class Path
	{
	public:
		static String GetFileName(const String& path);
		static String GetFileNameWithoutExtension(const String& path);

		/**
			Returns the extension part of the path, including the prefixed dot. An empty string is returned if the path has no extension part
		*/
		static String GetExtension(const String& path);

		/**
			Returns the directory part of the path, without the trailing slash. '.' is returned if the path has no directory part
		*/
		static String GetDirectoryName(const String& path);


		static String Parse(const String& path);
		static String GetRelativePath(const String& path, const String& dir);
		static String GetAbsolutePath(const String& path, const String& dir);
	};
}


#endif
