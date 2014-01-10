#include <Xli/Disk.h>
#include <Xli/Path.h>
#include <Xli/Random.h>
#include <Xli/Unicode.h>
#include <Xli/PlatformSpecific/Win32Header.h>
#include <Xli/PlatformSpecific/Win32Helpers.h>
#include <ShlObj.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        static UInt64 ConvertToUInt64(const DWORD& high, const DWORD& low)
        {
            return ((UInt64)high << 32) | low;
        }

        static Timestamp ConvertToTimestamp(const FILETIME& ft)
        {
            return ConvertToUInt64(ft.dwHighDateTime, ft.dwLowDateTime);
        }

        class Win32FileSystem : public NativeFileSystem
        {
        public:
            Stream* OpenFile(const String& filename, FileMode mode)
            {
                return new File(filename, mode);
            }

            virtual String CreateTempFilename()
            {
                Random rand(GetTickCount());
                static const char* cs = "-_1234567890abcdefghijklmnopqrstuvwxyz";
                int cl = (int)strlen(cs);

                WCHAR preW[] =
                {
                    (WCHAR)(cs[rand.NextInt(cl)]),
                    (WCHAR)(cs[rand.NextInt(cl)]),
                    (WCHAR)(cs[rand.NextInt(cl)]),
                    0
                };

                WCHAR pathW[MAX_PATH];
                WCHAR fnW[MAX_PATH];
                GetTempPathW(MAX_PATH, pathW);
                GetTempFileNameW(pathW, preW, 0, fnW);

                String result = Unicode::Utf16To8(fnW);

                for (int i = 0; i < result.Length(); i++)
                if (result[i] == '\\')
                    result[i] = '/';

                return result;
            }

            static String GetKnownDirectory(REFKNOWNFOLDERID rfid)
            {
                String result = ".";
                PWSTR pszPathW;

                if (::SHGetKnownFolderPath(rfid, KF_FLAG_NO_ALIAS, 0, &pszPathW) == S_OK)
                {
                    result = Unicode::Utf16To8(pszPathW);
                    CoTaskMemFree(pszPathW);
                }

                for (int i = 0; i < result.Length(); i++)
                if (result[i] == '\\')
                    result[i] = '/';

                return result;
            }

            virtual String GetSystemDirectory(SystemDirectory dir)
            {
                switch (dir)
                {
                case SystemDirectoryDocuments:
                    return GetKnownDirectory(FOLDERID_Documents);

                case SystemDirectoryRoamingAppData:
                    return GetKnownDirectory(FOLDERID_RoamingAppData);

                case SystemDirectoryLocalAppData:
                    return GetKnownDirectory(FOLDERID_LocalAppData);
                }

                XLI_THROW("Unknown system directory: " + (String)dir);
            }

            virtual String GetCurrentDirectory()
            {
                WCHAR bufW[4096];
                ::GetCurrentDirectoryW(4096, bufW);

                String result = Unicode::Utf16To8(bufW);

                for (int i = 0; i < result.Length(); i++)
                if (result[i] == '\\')
                    result[i] = '/';

                return result;
            }

            virtual void ChangeDirectory(const String& dir)
            {
                Utf16String dirW = Unicode::Utf8To16(dir);
                ::SetCurrentDirectoryW(dirW.Data());
            }

            virtual void CreateDirectory(const String& path)
            {
                // silent ignore on disk roots
                if (path.EndsWith(":")) return;

                Utf16String pathW = Unicode::Utf8To16(path);

                if (!CreateDirectoryW(pathW.Data(), 0))
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
                Utf16String nameW = Unicode::Utf8To16(name);

                if (!RemoveDirectoryW(nameW.Data()))
                {
                    XLI_THROW("Unable to delete directory '" + name + "': " + Win32Helpers::GetLastErrorString());
                }
            }

            virtual void DeleteFile(const String& name)
            {
                Utf16String nameW = Unicode::Utf8To16(name);

                if (!DeleteFileW(nameW.Data()))
                    XLI_THROW("Unable to delete file '" + name + "': " + Win32Helpers::GetLastErrorString());
            }

            virtual void MoveDirectory(const String& oldName, const String& newName)
            {
                Utf16String oldNameW = Unicode::Utf8To16(oldName);
                Utf16String newNameW = Unicode::Utf8To16(newName);

                if (!MoveFileW(oldNameW.Data(), newNameW.Data()))
                    XLI_THROW("Unable to move directory '" + oldName + "' to '" + newName + "': " + Win32Helpers::GetLastErrorString());
            }

            virtual void MoveFile(const String& oldName, const String& newName)
            {
                Utf16String oldNameW = Unicode::Utf8To16(oldName);
                Utf16String newNameW = Unicode::Utf8To16(newName);

                if (!MoveFileW(oldNameW.Data(), newNameW.Data()))
                    XLI_THROW("Unable to move file '" + oldName + "' to '" + newName + "': " + Win32Helpers::GetLastErrorString());
            }

            virtual bool GetFileInfo(const String& path, FileInfo& info)
            {
                Utf16String pathW = Unicode::Utf8To16(path);

                WIN32_FILE_ATTRIBUTE_DATA data;

                if (GetFileAttributesEx(pathW.Data(), GetFileExInfoStandard, &data))
                {
                    info.Name = path;
                    info.Flags = 0;
                    info.LastAccessTime = ConvertToTimestamp(data.ftLastAccessTime);
                    info.LastWriteTime = ConvertToTimestamp(data.ftLastWriteTime);
                    info.CreationTime = ConvertToTimestamp(data.ftCreationTime);
                    info.Size = ConvertToUInt64(data.nFileSizeHigh, data.nFileSizeLow);
                    if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) info.Flags |= FileFlagReadOnly;
                    if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) info.Flags |= FileFlagDirectory;
                    return true;
                }

                return false;
            }

            virtual void GetFiles(const String& path, Array<FileInfo>& list)
            {
                String filter = path.Length() ?
                    path + (path.Last() == '/' ? "*" : "/*") :
                    "*";

                Utf16String filterW = Unicode::Utf8To16(filter);

                WIN32_FIND_DATA findData;
                HANDLE h = FindFirstFile((LPCWSTR)filterW.Data(), &findData);

                if (h != INVALID_HANDLE_VALUE)
                {
                    do
                    {
                        FileInfo info;
                        info.Name = Unicode::Utf16To8(findData.cFileName);

                        if (info.Name == "." || info.Name == "..")
                            continue;

                        if (path.Length() && path != "." && path != "./")
                        {
                            if (path.Last() == '/')
                                info.Name = path + info.Name;
                            else
                                info.Name = path + "/" + info.Name;
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
    }

    NativeFileSystem* CreateNativeFileSystem()
    {
        return new PlatformSpecific::Win32FileSystem();
    }

    FileSystem* CreateBundleFileSystem()
    {
        const char* possibleDataSuffixes[] =
        {
            "/data",
            "/../data",
            "/../../data",
            "/../../../data",
        };

        const size_t BufSize = 4096;

        WCHAR buf[BufSize];
        DWORD result = GetModuleFileNameW(GetModuleHandle(0), buf, BufSize);

        if (result > 0 && result < BufSize)
        {
            String exe = Unicode::Utf16To8(buf, (int)result);

            for (int i = 0; i < exe.Length(); i++)
                if (exe[i] == '\\')
                    exe[i] = '/';

            String dir = Path::GetDirectoryName(exe);

            for (int i = 0; i < sizeof(possibleDataSuffixes); i++)
                if (Disk->IsDirectory(dir + possibleDataSuffixes[i]))
                    return Disk->CreateSubFileSystem(dir + possibleDataSuffixes[i]);
        }

        return Disk->CreateSubFileSystem(Disk->GetCurrentDirectory() + "/data");
    }
}
