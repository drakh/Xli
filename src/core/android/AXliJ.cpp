#include "AInternal.h"
#include "AXliJ.h"

namespace Xli
{
    void XliJ::MakeNoise() {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "makeNoise", "()V");
        jni->CallObjectMethod(shim_class, mid);
    }

    void XliJ::RaiseSoftKeyboard() {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "raiseKeyboard", "(Landroid/app/NativeActivity;)V");
        jobject activity = jni.GetInstance();
        jni->CallObjectMethod(shim_class, mid, activity);
    }

    void XliJ::HideSoftKeyboard() {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "hideKeyboard", "(Landroid/app/NativeActivity;)V");
        jobject activity = jni.GetInstance();
        jni->CallObjectMethod(shim_class, mid, activity);
    }

    int XliJ::ShowMessageBox(const String& message, const String& caption, int buttons, int hints) {
        //setup for call
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "ShowMessageBox", "(Landroid/app/NativeActivity;Ljava/lang/CharSequence;Ljava/lang/CharSequence;II)I");

        //vars for call
        jobject activity = jni.GetInstance();
        jstring jcaption = jni->NewStringUTF(caption.Data());
        jstring jmessage = jni->NewStringUTF(message.Data());

        //call
        int result = (int)jni->CallObjectMethod(shim_class, mid, activity, jmessage, jcaption, (jint)buttons, (jint)hints);
        return result;
    }
}
