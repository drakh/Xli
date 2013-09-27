#include "AInternal.h"
#include "AXliJ.h"
#include <Xli/MessageBox.h>
#include <Xli/Console.h>

namespace Xli
{
	DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
	{
		JniHelper jni;
        XliJ::MakeNoise();
        XliJ::ShowMessageBox();
        return (DialogResult)0;
	}
}
