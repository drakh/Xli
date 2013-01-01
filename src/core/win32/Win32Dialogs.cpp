#include <Xli/Dialogs.h>
#include <Xli/NativeFileSystem.h>
#include <Xli/StringBuilder.h>
#include <Xli/Path.h>
#include <Xli/MessageBox.h>
#include <Xli/Win32Header.h>
#include <CommDlg.h>

namespace Xli
{
	static String FixExtension(const String& str)
	{
		return str.Trim('.').Trim('*');
	}

	static void InitOptions(Window* parent, const Dialogs::FileDialogOptions& options, bool mustExist, OPENFILENAMEW& ofn, WCHAR fnbuf[4096], String& filter, String& def, String& dir, String& cd)
	{
		fnbuf[0] = '\0';

		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
		ofn.lStructSize = sizeof(OPENFILENAMEW);

		def = FixExtension(options.DefaultExtension);
		dir = options.DefaultFolder;

		for (int i = 0; i < dir.Length(); i++)
		{
			if (dir[i] == '/') dir[i] = '\\';
		}

		if (options.FileExtensions.Length())
		{
			ofn.nFilterIndex = 1;

			StringBuilder fb;
			for (int i = 0; i < options.FileExtensions.Length(); i++)
			{
				String ext = FixExtension(options.FileExtensions[i].Extension);

				if (ext.Length()) ext = "*." + ext;
				else ext = "*.*";

				fb.Append(options.FileExtensions[i].Description);
				fb.Append(" (" + ext + ")");
				fb.AppendChar('\0');
				fb.Append(ext);
				fb.AppendChar('\0');

				if (options.FileExtensions[i].Extension == options.DefaultExtension)
				{
					ofn.nFilterIndex = i + 1;
				}
			}

			fb.AppendChar('\0');
			filter = fb.GetString();
		}
		else
		{
			ofn.nFilterIndex = 1;
			filter = String("All files (*.*)\0*.*\0\0", 21);
		}
		
		if (options.DefaultFile.Length())
		{
			memcpy(fnbuf, options.DefaultFile.Data(), options.DefaultFile.Length() * 2 + 2);
		}
		
		if (parent)
		{
			ofn.hwndOwner = (HWND)parent->GetNativeHandle();
		}

		ofn.hInstance = GetModuleHandle(NULL);
		ofn.lpstrFilter = (LPWSTR)filter.Data();
		ofn.lpstrFile = fnbuf;
		ofn.nMaxFile = 4096;
		ofn.lpstrInitialDir = (LPWSTR)dir.Data();
		ofn.lpstrTitle = (LPWSTR)options.Caption.Data();

		if (mustExist)
		{
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
		}
		else
		{
			ofn.Flags = OFN_OVERWRITEPROMPT | OFN_ENABLESIZING;
		}

		ofn.lpstrDefExt = (LPWSTR)def.Data();

		cd = Disk->GetCurrentDirectory();
	}

	static bool EndFileDialog(bool ret, WCHAR* fnbuf, const String& cd, String& result)
	{
		if (ret)
		{
			result = (Utf16Char*)fnbuf;

			for (int i = 0; i < result.Length(); i++)
			{
				if (result[i] == '\\') result[i] = '/';
			}
		}

		Disk->ChangeDirectory(cd);
		return ret;
	}

	bool Dialogs::OpenFile(Window* parent, const FileDialogOptions& options, String& result)
	{
		WCHAR fnbuf[4096];
		OPENFILENAMEW ofn;
		String filter, def, dir, cd;
		InitOptions(parent, options, true, ofn, fnbuf, filter, def, dir, cd);
		return EndFileDialog(GetOpenFileNameW(&ofn) == TRUE, fnbuf, cd, result);
	}

	bool Dialogs::SaveFile(Window* parent, const FileDialogOptions& options, String& result)
	{
		WCHAR fnbuf[4096];
		OPENFILENAMEW ofn;
		String filter, def, dir, cd;
		InitOptions(parent, options, false, ofn, fnbuf, filter, def, dir, cd);
		return EndFileDialog(GetSaveFileNameW(&ofn) == TRUE, fnbuf, cd, result);
	}
}
