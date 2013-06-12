#include "AInternal.h"

#include <Xli/MessageBox.h>
#include <Xli/Console.h>

namespace Xli
{
	DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
	{
		Err->WriteLine("MessageBox [" + caption + "]: " + message);

		switch (buttons)
		{
		case DialogButtonsOK: return DialogResultOK;
		case DialogButtonsYesNo: return DialogResultNo;
		default: return DialogResultCancel;
		}
	}
}
