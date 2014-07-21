#include <XliPlatform/PlatformSpecific/Android.h>
#include "AJniHelper.h"
#include "AShim.h"
#include <XliPlatform/MessageBox.h>
#include <Xli/Console.h>

namespace Xli
{
    DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
    {
        // Also print to log
        PrintLine("MessageBox [" + caption + "]: " + message);

        PlatformSpecific::AJniHelper jni;
        int result = PlatformSpecific::AShim::ShowMessageBox(message, caption, (int)buttons, (int)hints);
        return (DialogResult)result;
    }
}
