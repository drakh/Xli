#include "AInternal.h"
#include "AXliJ.h"
#include <Xli/MessageBox.h>
#include <Xli/Console.h>

namespace Xli
{
	DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
	{
		JniHelper jni;
        int result = XliJ::ShowMessageBox(message, caption, (int)buttons, (int)hints);
        return (DialogResult)result;
	}
}
