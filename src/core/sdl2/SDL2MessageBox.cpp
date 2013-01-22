#include <Xli/MessageBox.h>
#include <Xli/SDL2Window.h>

namespace Xli
{
	DialogResult MessageBox::Show(Window* wnd, const String& message, const String& caption, DialogButtons buttons, int hints)
	{
		SDL_MessageBoxData data;
		SDL_MessageBoxButtonData buttondata[3];

		data.flags = 0;
		data.window = wnd != 0 && wnd->GetPlatform() == PlatformSDL2 ? ((SDL2Window*)wnd)->GetSDLWindow() : 0;
		data.title = caption.Data();
		data.message = message.Data();
		data.numbuttons = 0;
		data.buttons = buttondata;
		data.colorScheme = 0;

		switch (buttons)
		{
		case DialogButtonsOK:
			buttondata[0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
			buttondata[0].buttonid = 0;
			buttondata[0].text = "OK";
			data.numbuttons = 1;
			break;

		case DialogButtonsOKCancel: 
			buttondata[0].flags = 0;
			buttondata[0].buttonid = 0;
			buttondata[0].text = "OK";
			buttondata[1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
			buttondata[1].buttonid = 1;
			buttondata[1].text = "Cancel";
			data.numbuttons = 2;
			break;

		case DialogButtonsYesNo: 
			buttondata[0].flags = 0;
			buttondata[0].buttonid = 2;
			buttondata[0].text = "Yes";
			buttondata[1].flags = 0;
			buttondata[1].buttonid = 3;
			buttondata[1].text = "No";
			data.numbuttons = 2;
			break;

		case DialogButtonsYesNoCancel: 
			buttondata[0].flags = 0;
			buttondata[0].buttonid = 2;
			buttondata[0].text = "Yes";
			buttondata[1].flags = 0;
			buttondata[1].buttonid = 3;
			buttondata[1].text = "No";
			buttondata[2].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
			buttondata[2].buttonid = 1;
			buttondata[2].text = "Cancel";
			data.numbuttons = 3;
			break;

		case DialogButtonsCancelTryContinue: 
			buttondata[0].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
			buttondata[0].buttonid = 1;
			buttondata[0].text = "Cancel";
			buttondata[1].flags = 0;
			buttondata[1].buttonid = 4;
			buttondata[1].text = "Try Again";
			buttondata[2].flags = 0;
			buttondata[2].buttonid = 5;
			buttondata[2].text = "Continue";
			data.numbuttons = 3;
			break;

		default:
			XLI_THROW("Invalid button combination for MessageDialog");
		}

		if (hints & DialogHintError) data.flags |= SDL_MESSAGEBOX_ERROR;
		else if (hints & DialogHintInformation) data.flags |= SDL_MESSAGEBOX_INFORMATION;
		else if (hints & DialogHintWarning) data.flags |= SDL_MESSAGEBOX_WARNING;
		
		if (hints & DialogHintButton1Default) buttondata[0].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		else if (hints & DialogHintButton2Default) buttondata[1].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		else if (hints & DialogHintButton3Default) buttondata[2].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;

		int result = -1;
		if (SDL_ShowMessageBox(&data, &result) == 0)
		{
			switch (result)
			{
			case 0: return DialogResultOK;
			case 1: return DialogResultCancel;
			case 2: return DialogResultYes;
			case 3: return DialogResultNo;
			case 4: return DialogResultTryAgain;
			case 5: return DialogResultContinue;
			}

			return DialogResultCancel;
		}

		XLI_THROW("MessageDialog failed");
	}
}
