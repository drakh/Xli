#include "AInternal.h"
#include "AXliJ.h"
#include "AStream.h"
#include "Xli/Window.h"

extern Xli::WindowEventHandler* GlobalEventHandler;
extern Xli::Window* GlobalWindow;

namespace Xli
{

    extern "C"
    {
        void JNICALL XliJ_OnKeyUp (JNIEnv *env , jobject obj, jint keyCode) 
        {
            Xli::Key key = XliJ::AndroidToXliKeyEvent((XliJ::AKeyEvent)keyCode);
            GlobalEventHandler->OnKeyUp(GlobalWindow, key);
        }

        void JNICALL XliJ_OnTextInput (JNIEnv *env , jobject obj, jstring keyChars) 
        {
            const char* jChars = env->GetStringUTFChars((jstring)keyChars, NULL);
            String xliChars = jChars;
            GlobalEventHandler->OnTextInput(GlobalWindow, xliChars);
            env->ReleaseStringUTFChars((jstring)keyChars, jChars);
        }

        void JNICALL XliJ_OnKeyDown (JNIEnv *env , jobject obj, jint keyCode) 
        {
            Xli::Key key = XliJ::AndroidToXliKeyEvent((XliJ::AKeyEvent)keyCode);
            GlobalEventHandler->OnKeyDown(GlobalWindow, key);
        }

        void AttachNativeCallbacks(jclass* shim_class, JNIEnv *l_env)
        {
            LOGD("Registering native functions");
            static JNINativeMethod native_funcs[] = {
                {(char* const)"XliJ_OnKeyUp", (char* const)"(I)V", (void *)&XliJ_OnKeyUp},
                {(char* const)"XliJ_OnKeyDown", (char* const)"(I)V", (void *)&XliJ_OnKeyDown},
                {(char* const)"XliJ_OnTextInput", (char* const)"(Ljava/lang/String;)V", (void *)&XliJ_OnTextInput},
            };
            // the last argument is the number of native functions
            jint attached = l_env->RegisterNatives(*shim_class, native_funcs, 3);
            if (attached < 0) {
                LOGE("COULD NOT REGISTER NATIVE FUNCTIONS");
            } else {
                LOGD("Native functions registered");
            }
        }

        void AttachHiddenView(jclass* shim_class, JNIEnv* env, jobject activity) 
        {
            jmethodID mid = env->GetStaticMethodID(*shim_class, "AttachHiddenView", "(Landroid/app/NativeActivity;)I");
            jint result = (jint)env->CallObjectMethod(*shim_class, mid, activity);
            if (!result) LOGE("Could not AttachHidden View (c++ side)");
        }
    }

    int XliJ::kbVisible = 0;

    void XliJ::HandleSpecialAndroidKeyEvents(XliJ::AKeyEvent androidKeyCode) 
    {
        switch (androidKeyCode)
        {
        case XliJ::KEYCODE_BACK:
            XliJ::kbVisible = 0;
            break;  
        };          
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
        kbVisible = 1;
    }

    void XliJ::HideSoftKeyboard()
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "hideKeyboard", "(Landroid/app/NativeActivity;)V");
        jobject activity = jni.GetInstance();
        jni->CallObjectMethod(shim_class, mid, activity);
        kbVisible = 0;
    }

    bool XliJ::KeyboardVisible()
    {
        return kbVisible;
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

    bool XliJ::ConnectedToNetwork()
    {
        //setup for call
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "ConnectedToNetwork", "(Landroid/app/NativeActivity;)Z");
        jobject activity = jni.GetInstance();
        jobject jresult = jni->CallObjectMethod(shim_class, mid, activity);
        bool result = (bool)jresult;
        jni->DeleteLocalRef(jresult);
        jni->DeleteLocalRef(activity);
        return (bool)result;
    }

    jobject XliJ::HttpNewConnection(const String& uri, const String& method, bool hasPayload)
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "NewHttpConnection", "(Ljava/lang/String;Ljava/lang/String;Z)Ljava/net/HttpURLConnection;");
        if (!mid) {
            LOGE("Unable to get NewHttpConnection mid");
            return NULL;
        }
        jstring uriString = jni->NewStringUTF(uri.Data());
        jstring methodString = jni->NewStringUTF(method.Data());        
        jobject result = jni->CallObjectMethod(shim_class, mid, uriString, methodString, (jboolean)hasPayload);
        jni->DeleteLocalRef(uriString);
        jni->DeleteLocalRef(methodString);
        return result;        
    }

    void XliJ::HttpCloseConnection(jobject httpConnection)
    {
        JniHelper jni;
        jmethodID mid = jni.GetInstanceMethod(httpConnection, "disconnect", "()V");
        if (!mid) {
            LOGE("Unable to disconnect, cant get mid");
            return; 
        }
        jni->CallObjectMethod(httpConnection, mid);
    }

    AStream* XliJ::HttpGetOutputStream(jobject httpConnection)
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "HttpGetOutputStream", "(Ljava/net/HttpURLConnection;)Ljava/io/OutputStream;");
        if (!mid) {
            LOGE("Unable to get HttpGetOutputStream mid");
            return NULL;
        }
        jobject jStream = jni->CallObjectMethod(shim_class, mid, httpConnection);
        if (jStream) {
            jni->NewGlobalRef(jStream);
            return new AStream(AStream::WRITE, jStream);
        } else {
            return new AStream();
        }
    }

    AStream* XliJ::HttpGetInputStream(jobject httpConnection)
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "HttpGetInputStream", "(Ljava/net/HttpURLConnection;)Ljava/io/InputStream;");
        if (!mid) {
            LOGE("Unable to get HttpGetInputStream mid");
            return new AStream();
        }
        jobject jStream = jni->CallObjectMethod(shim_class, mid, httpConnection);
        if (jStream) {
            jni->NewGlobalRef(jStream);
            return new AStream(AStream::READ, jStream);
        } else {
            return new AStream();
        }
    }

    void XliJ::HttpSetHeader(jobject httpConnection, const String& key, const String& val)
    {
        JniHelper jni;
        jmethodID mid = jni.GetInstanceMethod(httpConnection, "addRequestProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
        if (!mid) {
            LOGE("Unable to get add header, cant get mid");
            return; 
        }
        jstring jKey = jni->NewStringUTF(key.Data());
        jstring jVal = jni->NewStringUTF(val.Data());
        jni->CallObjectMethod(httpConnection, mid, jKey, jVal);
        jni->DeleteLocalRef(jKey);
        jni->DeleteLocalRef(jVal);
    }

    String XliJ::HttpGetHeader(jobject httpConnection, const String& key)
    {
        JniHelper jni;
        jmethodID mid = jni.GetInstanceMethod(httpConnection, "getHeaderField", "(Ljava/lang/String;)Ljava/lang/String;");
        if (!mid) LOGE("Unable to get add header, cant get mid");
        jstring jKey = jni->NewStringUTF(key.Data());
        jobject val = jni->CallObjectMethod(httpConnection, mid, jKey);
        if (val) {
            jni->DeleteLocalRef(jKey);
            String result = jni.GetString((jstring)val);
            jni->DeleteLocalRef(val);
            return result;
        }
        return String("<invalid>");
    }

    int XliJ::GetResponseCode(jobject httpConnection)
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "getResponseCode",
                                               "(Ljava/net/HttpURLConnection;)I");
        if (!mid) LOGE("Unable to get GetResponseCode, cant get mid");
        jobject jobj = jni->CallObjectMethod(shim_class, mid, httpConnection);
        int result = (int)jobj;
        jni->DeleteLocalRef(jobj);
        return result;
    }

    void XliJ::HttpShowHeaders(jobject httpConnection)
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "HttpShowHeaders",
                                               "(Ljava/net/HttpURLConnection;)V");
        if (!mid) LOGE("Unable to get httpshowheaders, cant get mid");
        jni->CallObjectMethod(shim_class, mid, httpConnection);
    }

    void XliJ::InitDefaultCookieManager()
    {
        JniHelper jni;
        jclass shim_class = jni.GetShim();
        jmethodID mid = jni->GetStaticMethodID(shim_class, "InitDefaultCookieManager", "()V");
        if (!mid) LOGE("Unable to get InitDefaultCookieManager, cant get mid");
        jni->CallObjectMethod(shim_class, mid);
    }

    Xli::Key XliJ::AndroidToXliKeyEvent(XliJ::AKeyEvent androidKeyCode) 
    {
        switch (androidKeyCode)
        {
        case XliJ::KEYCODE_0:
            return Key0;
        case XliJ::KEYCODE_1:
            return Key1;
        case XliJ::KEYCODE_2:
            return Key2;
        case XliJ::KEYCODE_3:
            return Key3;
        case XliJ::KEYCODE_4:
            return Key4;
        case XliJ::KEYCODE_5:
            return Key5;
        case XliJ::KEYCODE_6:
            return Key6;
        case XliJ::KEYCODE_7:
            return Key7;
        case XliJ::KEYCODE_8:
            return Key8;
        case XliJ::KEYCODE_9:
            return Key9;
        case XliJ::KEYCODE_A:
            return KeyA;
        case XliJ::KEYCODE_ALT_LEFT:
            return KeyAlt;
        case XliJ::KEYCODE_B:
            return KeyB;
        case XliJ::KEYCODE_BACK:
            return KeyUnknown;
        case XliJ::KEYCODE_BREAK:
            return KeyBreak;
        case XliJ::KEYCODE_C:
            return KeyC;
        case XliJ::KEYCODE_CAPS_LOCK:
            return KeyCapsLock;
        case XliJ::KEYCODE_CTRL_LEFT:
            return KeyCtrl;
        case XliJ::KEYCODE_D:
            return KeyD;
        case XliJ::KEYCODE_DEL:
            return KeyBackspace;
        case XliJ::KEYCODE_DPAD_DOWN:
            return KeyDown;
        case XliJ::KEYCODE_DPAD_LEFT:
            return KeyLeft;
        case XliJ::KEYCODE_DPAD_RIGHT:
            return KeyRight;
        case XliJ::KEYCODE_DPAD_UP:
            return KeyUp;
        case XliJ::KEYCODE_E:
            return KeyE;
        case XliJ::KEYCODE_ENDCALL:
            return KeyEnd;
        case XliJ::KEYCODE_ENTER:
            return KeyEnter;
        case XliJ::KEYCODE_ESCAPE:
            return KeyEscape;
        case XliJ::KEYCODE_F:
            return KeyF;
        case XliJ::KEYCODE_F1:
            return KeyF1;
        case XliJ::KEYCODE_F10:
            return KeyF10;
        case XliJ::KEYCODE_F11:
            return KeyF11;
        case XliJ::KEYCODE_F12:
            return KeyF12;
        case XliJ::KEYCODE_F2:
            return KeyF2;
        case XliJ::KEYCODE_F3:
            return KeyF3;
        case XliJ::KEYCODE_F4:
            return KeyF4;
        case XliJ::KEYCODE_F5:
            return KeyF5;
        case XliJ::KEYCODE_F6:
            return KeyF6;
        case XliJ::KEYCODE_F7:
            return KeyF7;
        case XliJ::KEYCODE_F8:
            return KeyF8;
        case XliJ::KEYCODE_F9:
            return KeyF9;
        case XliJ::KEYCODE_G:
            return KeyG;
        case XliJ::KEYCODE_H:
            return KeyH;
        case XliJ::KEYCODE_HOME:
            return KeyHome;
        case XliJ::KEYCODE_I:
            return KeyI;
        case XliJ::KEYCODE_INSERT:
            return KeyInsert;
        case XliJ::KEYCODE_J:
            return KeyJ;
        case XliJ::KEYCODE_K:
            return KeyK;
        case XliJ::KEYCODE_L:
            return KeyL;
        case XliJ::KEYCODE_M:
            return KeyM;
        case XliJ::KEYCODE_MENU:
            return KeyMenu;
        case XliJ::KEYCODE_N:
            return KeyN;
        case XliJ::KEYCODE_NUMPAD_0:
            return KeyNumPad0;
        case XliJ::KEYCODE_NUMPAD_1:
            return KeyNumPad1;
        case XliJ::KEYCODE_NUMPAD_2:
            return KeyNumPad2;
        case XliJ::KEYCODE_NUMPAD_3:
            return KeyNumPad3;
        case XliJ::KEYCODE_NUMPAD_4:
            return KeyNumPad4;
        case XliJ::KEYCODE_NUMPAD_5:
            return KeyNumPad5;
        case XliJ::KEYCODE_NUMPAD_6:
            return KeyNumPad6;
        case XliJ::KEYCODE_NUMPAD_7:
            return KeyNumPad7;
        case XliJ::KEYCODE_NUMPAD_8:
            return KeyNumPad8;
        case XliJ::KEYCODE_NUMPAD_9:
            return KeyNumPad9;
        case XliJ::KEYCODE_O:
            return KeyO;
        case XliJ::KEYCODE_P:
            return KeyP;
        case XliJ::KEYCODE_PAGE_DOWN:
            return KeyPageDown;
        case XliJ::KEYCODE_PAGE_UP:
            return KeyPageUp;
        case XliJ::KEYCODE_Q:
            return KeyQ;
        case XliJ::KEYCODE_R:
            return KeyR;
        case XliJ::KEYCODE_S:
            return KeyS;
        case XliJ::KEYCODE_SHIFT_LEFT:
            return KeyShift;
        case XliJ::KEYCODE_SPACE:
            return KeySpace;
        case XliJ::KEYCODE_T:
            return KeyT;
        case XliJ::KEYCODE_TAB:
            return KeyTab;
        case XliJ::KEYCODE_U:
            return KeyU;
        case XliJ::KEYCODE_UNKNOWN:
            return KeyUnknown;
        case XliJ::KEYCODE_V:
            return KeyV;
        case XliJ::KEYCODE_W:
            return KeyW;
        case XliJ::KEYCODE_X:
            return KeyX;
        case XliJ::KEYCODE_Y:
            return KeyY;
        case XliJ::KEYCODE_Z:
            return KeyZ; 
        };
        return KeyUnknown;
    }
}
