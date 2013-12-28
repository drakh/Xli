#include <Xli/PlatformSpecific/Android.h>
#include <Xli/MessageBox.h>
#include <Xli/Console.h>

namespace Xli
{
    DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
    {
        PlatformSpecific::AJniHelper jni;
        int result = PlatformSpecific::AShim::ShowMessageBox(message, caption, (int)buttons, (int)hints);
        return (DialogResult)result;
    }
}
