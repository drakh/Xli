#include "AInternal.h"

#include <Xli/MessageBox.h>
#include <Xli/Console.h>

namespace Xli
{
	DialogResult MessageBox::Show(Window* parent, const String& message, const String& caption, DialogButtons buttons, int hints)
	{
		Out->WriteLine("MessageBox [" + caption + "]: " + message);
		
		JniHelper jni;

		jclass Xli_class = jni->GetObjectClass(AndroidActivity->clazz);
		jmethodID Xli_showMessageBox = jni->GetMethodID(Xli_class, "showMessageBox", "(Ljava/lang/String;Ljava/lang/String;II)I");

		if (Xli_showMessageBox)
		{
			jstring jniMessage = jni->NewStringUTF(message.Data());
			jstring jniCaption = jni->NewStringUTF(caption.Data());
			jint jniButtons = buttons;
			jint jniHints = hints;

			jint result = jni->CallStaticIntMethod(Xli_class, Xli_showMessageBox, jniMessage, jniCaption, jniButtons, jniHints);

			return (DialogResult)(int)result;
		}

		Err->WriteLine("JNI ERROR: showMessageBox() could not be called");

		switch (buttons)
		{
			// TODO: Return default button if one is set in hints

		case DialogButtonsOK: return DialogResultOK;
		case DialogButtonsYesNo: return DialogResultNo;
		default: return DialogResultCancel;
		}
	}
}
