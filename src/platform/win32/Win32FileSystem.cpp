#include <XliPlatform/NativeFileSystem.h>
#include <Xli/File.h>
#include <Xli/Random.h>
#include <XliPlatform/Win32Windows.h>
#include <XliPlatform/Win32Helpers.h>
#include <ShlObj.h>

namespace Xli
{
	static inline UInt64 ConvertToUInt64(const DWORD& high, const DWORD& low)
	{
		return ((UInt64)high << 32) | low;
	}

	static inline Timestamp ConvertToTimestamp(const FILETIME& ft)
	{
		return ConvertToUInt64(ft.dwHighDateTime, ft.dwLowDateTime);
	}

	class Win32FileSystem: public NativeFileSystem
	{
	public:
		Stream* OpenFile(const String& fileName, FileMode mode)
		{
			return new File(fileName, mode);
		}

		virtual String CreateTempFilename()
		{
			Random rand(GetTickCount());
			static const char* cs = "-_1234567890abcdefghijklmnopqrstuvwxyz";
			int cl = strlen(cs);

			TCHAR pre[] = 
			{
				(TCHAR)(cs[rand.NextInt(cl)]),
				(TCHAR)(cs[rand.NextInt(cl)]),
				(TCHAR)(cs[rand.NextInt(cl)]),
				0
			};

			TCHAR path[MAX_PATH];
			TCHAR fn[MAX_PATH];
			GetTempPath(MAX_PATH, path);
			GetTempFileName(path, pre, 0, fn);

			return (const TCHAR*)fn;
		}

		static String GetKnownDirectory(REFKNOWNFOLDERID rfid)
		{
			String result = ".";
			PWSTR pszPath;

			if (::SHGetKnownFolderPath(rfid, KF_FLAG_NO_ALIAS, 0, &pszPath) == S_OK)
			{
				result = (Utf16Char*)pszPath;
				CoTaskMemFree(pszPath);
			}

			for (int i = 0; i < result.Length(); i++)
			{
				if (result[i] == '\\') result[i] = '/';
			}

			return result;
		}

		virtual String GetDocumentsDirectory()
		{
			return GetKnownDirectory(FOLDERID_Documents);
		}

		virtual String GetRoamingAppDataDirectory()
		{
			return GetKnownDirectory(FOLDERID_RoamingAppData);
		}

		virtual String GetLocalAppDataDirectory()
		{
			return GetKnownDirectory(FOLDERID_LocalAppData);
		}

		virtual String GetCurrentDirectory()
		{
			WCHAR buf[4096];
			::GetCurrentDirectoryW(4096, buf);

			for (int i = 0; buf[i]; i++)
			{
				if (buf[i] == '\\') buf[i] = '/';
			}

			return (Utf16Char*)buf;
		}

		virtual void ChangeDirectory(const String& dir) 
		{
			::SetCurrentDirectoryW((LPCWSTR)dir.Data());
		}

		virtual void CreateDirectory(const String& path)
		{
			// silent ignore on disk roots
			if (path.EndsWith(":")) return;

			if (!CreateDirectoryW((LPCWSTR)path.Data(), 0))
			{
				DWORD err = GetLastError();
				switch (err)
				{
				case ERROR_ALREADY_EXISTS:
					break;
				case ERROR_PATH_NOT_FOUND:
				default:
					XLI_THROW("Couldn't create directory: " + path);
				};
			}
		}

		virtual void DeleteDirectory(const String& name)
		{ 
			if (!RemoveDirectoryW((LPCWSTR)name.Data()))
			{
				XLI_THROW("Unable to delete directory '" + name + "': " + Win32Helpers::GetLastErrorString());
			}
		}

		virtual void DeleteFile(const String& name)
		{ 
			if (!DeleteFileW((LPCWSTR)name.Data()))
			{
				XLI_THROW("Unable to delete file '" + name + "': " + Win32Helpers::GetLastErrorString());
			}
		}

		virtual void MoveDirectory(const String& oldName, const String& newName)
		{ 
			if (!MoveFileW((LPCWSTR)oldName.Data(), (LPCWSTR)newName.Data()))
			{
				XLI_THROW("Unable to move directory '" + oldName + "' to '" + newName + "': " + Win32Helpers::GetLastErrorString());
			}			
		}
		
		virtual void MoveFile(const String& oldName, const String& newName)
		{ 
			if (!MoveFileW((LPCWSTR)oldName.Data(), (LPCWSTR)newName.Data()))
			{
				XLI_THROW("Unable to move file '" + oldName + "' to '" + newName + "': " + Win32Helpers::GetLastErrorString());
			}
		}

		virtual bool FileExists(const String& path)
		{
			WIN32_FILE_ATTRIBUTE_DATA data;
			return GetFileAttributesExW((LPCWSTR)path.Data(), GetFileExInfoStandard, &data) == TRUE;
		}

		virtual FileInfo GetFileInfo(const String& path)
		{
			WIN32_FILE_ATTRIBUTE_DATA data;

			if (GetFileAttributesEx((LPCWSTR)path.Data(), GetFileExInfoStandard, &data))
			{
				FileInfo info;
				info.Name = path;
				info.Flags = 0;
				info.LastAccessTime = ConvertToTimestamp(data.ftLastAccessTime);
				info.LastWriteTime = ConvertToTimestamp(data.ftLastWriteTime);
				info.CreationTime = ConvertToTimestamp(data.ftCreationTime);
				info.Size = ConvertToUInt64(data.nFileSizeHigh, data.nFileSizeLow);
				if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) info.Flags |= FileFlagReadOnly;
				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) info.Flags |= FileFlagDirectory;

				return info;
			}

			XLI_THROW_FILE_NOT_FOUND(path);
		}

		virtual void GetFiles(const String& path, Array<FileInfo>& list)
		{
			WIN32_FIND_DATA findData;
			String filter;
			if (path.Length())
			{
				if (path.Last() == '/') filter = path + "*";
				else filter = path + "/*";
			}
			else filter = "*";

			HANDLE h = FindFirstFile((LPCWSTR)filter.Data(), &findData);

			if (h != INVALID_HANDLE_VALUE)
			{
				do
				{
					FileInfo info;
					info.Name = findData.cFileName;

					if (info.Name == "." || info.Name == "..")
					{
						continue;
					}

					if (path.Length() && path != "." && path != "./")
					{
						if (path.Last() == '/') info.Name = path + info.Name;
						else info.Name = path + "/" + info.Name;
					}

					info.Flags = 0;
					info.LastAccessTime = ConvertToTimestamp(findData.ftLastAccessTime);
					info.LastWriteTime = ConvertToTimestamp(findData.ftLastWriteTime);
					info.CreationTime = ConvertToTimestamp(findData.ftCreationTime);
					info.Size = ConvertToUInt64(findData.nFileSizeHigh, findData.nFileSizeLow);
					if (findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) info.Flags |= FileFlagReadOnly;
					if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) info.Flags |= FileFlagDirectory;

					list.Add(info);

				} while (FindNextFile(h, &findData) != 0);
			}

			FindClose(h);
		}
	};

	NativeFileSystem* CreateNativeFileSystem()
	{
		return new Win32FileSystem();
	}
}
