#include <Xli/Path.h>
#include <Xli/Exception.h>
#include <Xli/String.h>

namespace Xli
{
	String Path::GetFileName(const String& path)
	{
		for (int i = path.Length() - 1; i >= 0; i--)
		{
			if (path.Get(i) == '/')
			{
				return path.SubString(i + 1, path.Length() - i - 1);
			}
		}
		return path;
	}

	String Path::GetFileNameWithoutExtension(const String& path)
	{
		int extIndex = path.Length();

		for (int i = path.Length() - 1; i >= 0; i--)
		{
			if (path.Get(i) == '.')
			{
				extIndex = i;
			}
			else if (path.Get(i) == '/')
			{
				return path.SubString(i + 1, extIndex - i - 1);
			}
		}
		return path.SubString(0, extIndex);
	}

	String Path::GetExtension(const String& path)
	{
		for (int i = path.Length() - 1; i >= 0; i--)
		{
			if (path.Get(i) == '.')
			{
				return path.SubString(i, path.Length() - i);
			}
			else if (path.Get(i) == '/')
			{
				break;
			}
		}
		return "";
	}

	String Path::GetDirectoryName(const String& path)
	{
		for (int i = path.Length() - 1; i >= 0; i--)
		{
			if (path.Get(i) == '/')
			{
				return path.SubString(0, i > 0 ? i : 1);
			}
		}
		return ".";
	}

	String Path::Parse(const String& path)
	{
		// TODO
		return path;
	}

	String Path::GetRelativePath(const String& a, const String& b)
	{
		// TODO
		return a.StartsWith(b) ? a.SubString(b.Length() + 1, a.Length() - b.Length() - 1) : a;
	}

	String Path::GetAbsolutePath(const String& a, const String& b)
	{
		// TODO
		return b + "/" + a;
	}
}
