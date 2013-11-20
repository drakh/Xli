#include <Xli/Path.h>
#include <Xli/Exception.h>
#include <Xli/String.h>

namespace Xli
{
	static bool IsRooted(const String& path)
	{
		return 
			path.Length() > 1 && path[0] == '/' || 
			path.Length() > 2 && path[1] == ':';
	}

	String Path::Combine(const String& path1, const String& path2)
	{
		return 
			IsRooted(path2) ?
				path2 :
				path1.Length() && path1.Last() != '/' && path1.Last() != '\\' ?
					path1 + "/" + path2 :
					path1 + path2;
	}

	String Path::GetFilename(const String& path)
	{
		for (int i = path.Length() - 1; i >= 0; i--)
			if (path[i] == '/' || path[i] == '\\')
				return path.Substring(i + 1, path.Length() - i - 1);

		return path;
	}

	String Path::GetFilenameWithoutExtension(const String& path)
	{
		int extIndex = path.Length();

		for (int i = path.Length() - 1; i >= 0; i--)
			if (path[i] == '.')
				extIndex = i;
			else if (path[i] == '/' || path[i] == '\\')
				return path.Substring(i + 1, extIndex - i - 1);

		return path.Substring(0, extIndex);
	}

	String Path::GetExtension(const String& path)
	{
		for (int i = path.Length() - 1; i >= 0; i--)
			if (path[i] == '.')
				return path.Substring(i, path.Length() - i);
			else if (path[i] == '/' || path[i] == '\\')
				break;

		return "";
	}

	String Path::GetDirectoryName(const String& path)
	{
		for (int i = path.Length() - 1; i >= 0; i--)
			if (path[i] == '/' || path[i] == '\\')
				return path.Substring(0, i > 0 ? i : 1);

		return ".";
	}
}
