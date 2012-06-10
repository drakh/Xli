#include <XliPlatform/MessageBox.h>
#include <Xli/Console.h>

namespace Xli
{
	DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
	{
		PrintLine("");
        PrintLine("---------------------------");
        PrintLine(caption);
        PrintLine("---------------------------");
        PrintLine(message);
        PrintLine("---------------------------");

        DialogResult r;

		while (true)
		{
			switch (buttons)
			{
			case DialogButtonsOK: Print("OK"); break;
			case DialogButtonsOKCancel: Print("OK/Cancel"); break;
			case DialogButtonsYesNo: Print("Yes/No"); break;
			case DialogButtonsYesNoCancel: Print("Yes/No/Cancel"); break;
			case DialogButtonsCancelTryContinue: Print("Cancel/Retry/Continue"); break;
			}

            Print(": ");
            CharString in = In->ReadLine().Trim().ToLower();

			if ((buttons == DialogButtonsOK || buttons == DialogButtonsOKCancel) && (in == "ok" || in == "")) { r = DialogResultOK; break; }
			else if ((buttons == DialogButtonsOKCancel || buttons == DialogButtonsYesNoCancel || buttons == DialogButtonsCancelTryContinue) && (in == "cancel" || in == "c")) { r = DialogResultCancel; break; }
			else if ((buttons == DialogButtonsYesNo || buttons == DialogButtonsYesNoCancel) && (in == "yes" || in == "y")) { r = DialogResultYes; break; }
			else if ((buttons == DialogButtonsYesNo || buttons == DialogButtonsYesNoCancel) && (in == "no" || in == "n")) { r = DialogResultNo; break; }
			else if ((buttons == DialogButtonsCancelTryContinue) && (in == "retry" || in== "r")) { r = DialogResultTryAgain; break; }
			else if ((buttons == DialogButtonsCancelTryContinue) && (in == "continue")) { r = DialogResultContinue; break; }
		}

        PrintLine("---------------------------");

        return r;
	}
}
