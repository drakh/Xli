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

    void XliJ::ShowMessageBox() {
        //setup for call
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "ShowMessageBox", "(Landroid/app/NativeActivity;Ljava/lang/CharSequence;Ljava/lang/CharSequence;II)I");

        //vars for call
        jobject activity = jni.GetInstance();
        jstring caption = jni->NewStringUTF("lets go");
        jstring message = jni->NewStringUTF("here is a message");

        //call
        jni->CallObjectMethod(shim_class, mid, activity, message, caption, 0, 0);
    }
}
