#include "AInternal.h"
#include "AXliJ.h"
#include "Xli/Window.h"

namespace Xli
{

    extern "C"
    {
        void JNICALL XliJ_OnKey (JNIEnv *env , jobject obj, jint keyCode) 
        {
            LOGE("XliJ_OnKey: %d", XliJ::AndroidToXliKeyEvent((XliJ::AKeyEvent)keyCode));
        }
        void JNICALL XliJ_OnKeyUp (JNIEnv *env , jobject obj, jint keyCode) 
        {
            LOGE("XliJ_OnKeyUp: %d", XliJ::AndroidToXliKeyEvent((XliJ::AKeyEvent)keyCode));
        }
        void JNICALL XliJ_OnKeyDown (JNIEnv *env , jobject obj, jint keyCode) 
        {
            LOGE("XliJ_OnKeyDown: %d", XliJ::AndroidToXliKeyEvent((XliJ::AKeyEvent)keyCode));
        }
        void JNICALL XliJ_OnKeyMultiple (JNIEnv *env , jobject obj, jint keyCode, jint count) 
        {
            LOGE("XliJ_OnKeyMultiple: %d", XliJ::AndroidToXliKeyEvent((XliJ::AKeyEvent)keyCode));
        }
        void JNICALL XliJ_OnKeyLongPress (JNIEnv *env , jobject obj, jint keyCode)
        {
            LOGE("XliJ_OnKeyLongPress: %d", XliJ::AndroidToXliKeyEvent((XliJ::AKeyEvent)keyCode));
        }

        void AttachNativeCallbacks(jclass* shim_class, JNIEnv *l_env)
        {
            static JNINativeMethod native_funcs[] = {
                {(char* const)"XliJ_OnKey", (char* const)"(I)V", (void *)&XliJ_OnKey},
                {(char* const)"XliJ_OnKeyUp", (char* const)"(I)V", (void *)&XliJ_OnKeyUp},
                {(char* const)"XliJ_OnKeyDown", (char* const)"(I)V", (void *)&XliJ_OnKeyDown},
                {(char* const)"XliJ_OnKeyMultiple", (char* const)"(II)V", (void *)&XliJ_OnKeyMultiple},
                {(char* const)"XliJ_OnKeyLongPress", (char* const)"(I)V", (void *)&XliJ_OnKeyLongPress},
            };
            // the last argument is the number of native functions
            jint attached = l_env->RegisterNatives(*shim_class, native_funcs, 5);
            if (attached < 0) {
                LOGE("COULD NOT REGISTER NATIVE FUNCTIONS");
            }
        }
    }

    void XliJ::MakeNoise()
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "makeNoise", "()V");
        jni->CallObjectMethod(shim_class, mid);
    }

    void XliJ::RaiseSoftKeyboard()
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "raiseKeyboard", "(Landroid/app/NativeActivity;)V");
        jobject activity = jni.GetInstance();
        jni->CallObjectMethod(shim_class, mid, activity);
    }

    void XliJ::HideSoftKeyboard()
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "hideKeyboard", "(Landroid/app/NativeActivity;)V");
        jobject activity = jni.GetInstance();
        jni->CallObjectMethod(shim_class, mid, activity);
    }

    int XliJ::ShowMessageBox(const String& message, const String& caption, int buttons, int hints) 
    {
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

    int XliJ::AndroidToXliKeyEvent(XliJ::AKeyEvent androidKeyCode) 
    {
        switch (androidKeyCode)
        {
        case XliJ::KEYCODE_0:
        {
            return Key0;
        }
        case XliJ::KEYCODE_1:
        {
            return Key1;
        }
        case XliJ::KEYCODE_2:
        {
            return Key2;
        }
        case XliJ::KEYCODE_3:
        {
            return Key3;
        }
        case XliJ::KEYCODE_4:
        {
            return Key4;
        }
        case XliJ::KEYCODE_5:
        {
            return Key5;
        }
        case XliJ::KEYCODE_6:
        {
            return Key6;
        }
        case XliJ::KEYCODE_7:
        {
            return Key7;
        }
        case XliJ::KEYCODE_8:
        {
            return Key8;
        }
        case XliJ::KEYCODE_9:
        {
            return Key9;
        }
        case XliJ::KEYCODE_A:
        {
            return KeyA;
        }
        case XliJ::KEYCODE_ALT_LEFT:
        {
            return KeyAlt;
        }
        case XliJ::KEYCODE_B:
        {
            return KeyB;
        }
        case XliJ::KEYCODE_BACK:
        {
            return KeyBackspace;
        }
        case XliJ::KEYCODE_BREAK:
        {
            return KeyBreak;
        }
        case XliJ::KEYCODE_C:
        {
            return KeyC;
        }
        case XliJ::KEYCODE_CAPS_LOCK:
        {
            return KeyCapsLock;
        }
        case XliJ::KEYCODE_CTRL_LEFT:
        {
            return KeyCtrl;
        }
        case XliJ::KEYCODE_D:
        {
            return KeyD;
        }
        case XliJ::KEYCODE_DEL:
        {
            return KeyDelete;
        }
        case XliJ::KEYCODE_DPAD_DOWN:
        {
            return KeyDown;
        }
        case XliJ::KEYCODE_DPAD_LEFT:
        {
            return KeyLeft;
        }
        case XliJ::KEYCODE_DPAD_RIGHT:
        {
            return KeyRight;
        }
        case XliJ::KEYCODE_DPAD_UP:
        {
            return KeyUp;
        }
        case XliJ::KEYCODE_E:
        {
            return KeyE;
        }
        case XliJ::KEYCODE_ENDCALL:
        {
            return KeyEnd;
        }
        case XliJ::KEYCODE_ENTER:
        {
            return KeyEnter;
        }
        case XliJ::KEYCODE_ESCAPE:
        {
            return KeyEscape;
        }
        case XliJ::KEYCODE_F:
        {
            return KeyF;
        }
        case XliJ::KEYCODE_F1:
        {
            return KeyF1;
        }
        case XliJ::KEYCODE_F10:
        {
            return KeyF10;
        }
        case XliJ::KEYCODE_F11:
        {
            return KeyF11;
        }
        case XliJ::KEYCODE_F12:
        {
            return KeyF12;
        }
        case XliJ::KEYCODE_F2:
        {
            return KeyF2;
        }
        case XliJ::KEYCODE_F3:
        {
            return KeyF3;
        }
        case XliJ::KEYCODE_F4:
        {
            return KeyF4;
        }
        case XliJ::KEYCODE_F5:
        {
            return KeyF5;
        }
        case XliJ::KEYCODE_F6:
        {
            return KeyF6;
        }
        case XliJ::KEYCODE_F7:
        {
            return KeyF7;
        }
        case XliJ::KEYCODE_F8:
        {
            return KeyF8;
        }
        case XliJ::KEYCODE_F9:
        {
            return KeyF9;
        }
        case XliJ::KEYCODE_G:
        {
            return KeyG;
        }
        case XliJ::KEYCODE_H:
        {
            return KeyH;
        }
        case XliJ::KEYCODE_HOME:
        {
            return KeyHome;
        }
        case XliJ::KEYCODE_I:
        {
            return KeyI;
        }
        case XliJ::KEYCODE_INSERT:
        {
            return KeyInsert;
        }
        case XliJ::KEYCODE_J:
        {
            return KeyJ;
        }
        case XliJ::KEYCODE_K:
        {
            return KeyK;
        }
        case XliJ::KEYCODE_L:
        {
            return KeyL;
        }
        case XliJ::KEYCODE_M:
        {
            return KeyM;
        }
        case XliJ::KEYCODE_MENU:
        {
            return KeyMenu;
        }
        case XliJ::KEYCODE_N:
        {
            return KeyN;
        }
        case XliJ::KEYCODE_NUMPAD_0:
        {
            return KeyNumPad0;
        }
        case XliJ::KEYCODE_NUMPAD_1:
        {
            return KeyNumPad1;
        }
        case XliJ::KEYCODE_NUMPAD_2:
        {
            return KeyNumPad2;
        }
        case XliJ::KEYCODE_NUMPAD_3:
        {
            return KeyNumPad3;
        }
        case XliJ::KEYCODE_NUMPAD_4:
        {
            return KeyNumPad4;
        }
        case XliJ::KEYCODE_NUMPAD_5:
        {
            return KeyNumPad5;
        }
        case XliJ::KEYCODE_NUMPAD_6:
        {
            return KeyNumPad6;
        }
        case XliJ::KEYCODE_NUMPAD_7:
        {
            return KeyNumPad7;
        }
        case XliJ::KEYCODE_NUMPAD_8:
        {
            return KeyNumPad8;
        }
        case XliJ::KEYCODE_NUMPAD_9:
        {
            return KeyNumPad9;
        }
        case XliJ::KEYCODE_O:
        {
            return KeyO;
        }
        case XliJ::KEYCODE_P:
        {
            return KeyP;
        }
        case XliJ::KEYCODE_PAGE_DOWN:
        {
            return KeyPageDown;
        }
        case XliJ::KEYCODE_PAGE_UP:
        {
            return KeyPageUp;
        }
        case XliJ::KEYCODE_Q:
        {
            return KeyQ;
        }
        case XliJ::KEYCODE_R:
        {
            return KeyR;
        }
        case XliJ::KEYCODE_S:
        {
            return KeyS;
        }
        case XliJ::KEYCODE_SHIFT_LEFT:
        {
            return KeyShift;
        }
        case XliJ::KEYCODE_SPACE:
        {
            return KeySpace;
        }
        case XliJ::KEYCODE_T:
        {
            return KeyT;
        }
        case XliJ::KEYCODE_TAB:
        {
            return KeyTab;
        }
        case XliJ::KEYCODE_U:
        {
            return KeyU;
        }
        case XliJ::KEYCODE_UNKNOWN:
        {
            return KeyUnknown;
        }
        case XliJ::KEYCODE_V:
        {
            return KeyV;
        }
        case XliJ::KEYCODE_W:
        {
            return KeyW;
        }
        case XliJ::KEYCODE_X:
        {
            return KeyX;
        }
        case XliJ::KEYCODE_Y:
        {
            return KeyY;
        }
        case XliJ::KEYCODE_Z:
        {
            return KeyZ;
        }
        };
        return KeyUnknown;
    }
}
