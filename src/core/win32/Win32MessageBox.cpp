#include <Xli/MessageBox.h>
#include <Xli/Window.h>
#include <Xli/Win32Helpers.h>
#include <Xli/Win32Header.h>
#include <Xli/Unicode.h>

namespace Xli
{
	DialogResult MessageBox::Show(Window* wnd, const String& message, const String& caption, DialogButtons buttons, int hints)
	{
		HWND hWnd = 0;
		UINT type = 0;

		if (wnd)
		{
			hWnd = (HWND)wnd->GetNativeHandle();
		}

		switch (buttons)
		{
		case DialogButtonsOK: type |= MB_OK; break;
		case DialogButtonsOKCancel: type |= MB_OKCANCEL; break;
		case DialogButtonsYesNo: type |= MB_YESNO; break;
		case DialogButtonsYesNoCancel: type |= MB_YESNOCANCEL; break;
		case DialogButtonsCancelTryContinue: type |= MB_CANCELTRYCONTINUE; break;
		default:
			XLI_THROW("Invalid button combination for MessageDialog");
		}

		if (hints & DialogHintError) type |= MB_ICONERROR;
		if (hints & DialogHintInformation) type |= MB_ICONINFORMATION;
		if (hints & DialogHintQuestion) type |= MB_ICONQUESTION;
		if (hints & DialogHintWarning) type |= MB_ICONWARNING;
		if (hints & DialogHintButton1Default) type |= MB_DEFBUTTON1;
		if (hints & DialogHintButton2Default) type |= MB_DEFBUTTON2;
		if (hints & DialogHintButton3Default) type |= MB_DEFBUTTON3;

		Utf16String messageW = Unicode::Utf8To16(message);
		Utf16String captionW = Unicode::Utf8To16(caption);
		int result = MessageBoxW(hWnd, messageW.Data(), captionW.Data(), type);

		switch (result)
		{
		case IDOK: return DialogResultOK;
		case IDCANCEL: return DialogResultCancel;
		case IDCONTINUE: return DialogResultContinue;
		case IDIGNORE: return DialogResultContinue;
		case IDNO: return DialogResultNo;
		case IDTRYAGAIN: return DialogResultTryAgain;
		case IDYES: return DialogResultYes;
		default:
			XLI_THROW("MessageDialog failed: " + Xli::Win32Helpers::GetLastErrorString());
		}
	}
}
