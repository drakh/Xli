#include <Xli/MessageBox.h>
#include <Xli/SDL2Window.h>

namespace Xli
{
	DialogResult MessageBox::Show(Window* wnd, const String& message, const String& caption, DialogButtons buttons, int hints)
	{
		SDL_MessageBoxData data;
		SDL_MessageBoxButtonData buttons[3];

		data.flags = 0;
		data.window = ((SDL2Window*)wnd)->GetSDLWindow();
		data.title = caption.Data();
		data.message = message.Data();
		data.numbuttons = 0;
		data.buttons = buttons;
		data.colorScheme = 0;

		switch (buttons)
		{
		case DialogButtonsOK:
			buttons[0].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
			buttons[0].buttonid = 0;
			buttons[0].text = "OK";
			data.numbuttons = 1;
			break;

		case DialogButtonsOKCancel: 
			buttons[0].flags = 0;
			buttons[0].buttonid = 0;
			buttons[0].text = "OK";
			buttons[1].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
			buttons[1].buttonid = 1;
			buttons[1].text = "Cancel";
			data.numbuttons = 2;
			break;

		case DialogButtonsYesNo: 
			buttons[0].flags = 0;
			buttons[0].buttonid = 2;
			buttons[0].text = "Yes";
			buttons[1].flags = 0;
			buttons[1].buttonid = 3;
			buttons[1].text = "No";
			data.numbuttons = 2;
			break;

		case DialogButtonsYesNoCancel: 
			buttons[0].flags = 0;
			buttons[0].buttonid = 2;
			buttons[0].text = "Yes";
			buttons[1].flags = 0;
			buttons[1].buttonid = 3;
			buttons[1].text = "No";
			buttons[2].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
			buttons[2].buttonid = 1;
			buttons[2].text = "Cancel";
			data.numbuttons = 3;
			break;

		case DialogButtonsCancelTryContinue: 
			buttons[0].flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
			buttons[0].buttonid = 1;
			buttons[0].text = "Cancel";
			buttons[1].flags = 0;
			buttons[1].buttonid = 4;
			buttons[1].text = "Try Again";
			buttons[2].flags = 0;
			buttons[2].buttonid = 5;
			buttons[2].text = "Continue";
			data.numbuttons = 3;
			break;

		default:
			XLI_THROW("Invalid button combination for MessageDialog");
		}

		if (hints & DialogHintError) data.flags |= SDL_MESSAGEBOX_ERROR;
		else if (hints & DialogHintInformation) data.flags |= SDL_MESSAGEBOX_INFORMATION;
		else if (hints & DialogHintWarning) data.flags |= SDL_MESSAGEBOX_WARNING;
		
		if (hints & DialogHintButton1Default) data.buttons[0].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		else if (hints & DialogHintButton2Default) data.buttons[1].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		else if (hints & DialogHintButton3Default) data.buttons[2].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;

		int result = -1;
		if (SDL_ShowMessageBox(&data, &result))
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
