#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <Xli/PlatformSpecific/Android.h>
#include <Xli/Window.h>

namespace Xli
{
    namespace PlatformSpecific
    {
        int AShim::kbVisible = 0;

        void AShim::HandleSpecialAndroidKeyEvents(AKeyEvent androidKeyCode) 
        {
            switch (androidKeyCode)
            {
            case KEYCODE_BACK:
                AShim::kbVisible = 0;
                break;  
            };          
        }
        
        void AShim::MakeNoise()
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "makeNoise", "()V");
            jni->CallObjectMethod(shim_class, mid);
        }

        void AShim::RaiseSoftKeyboard()
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "raiseKeyboard", "(Landroid/app/NativeActivity;)V");
            jobject activity = jni.GetInstance();
            jni->CallObjectMethod(shim_class, mid, activity);
            kbVisible = 1;
        }

        void AShim::HideSoftKeyboard()
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "hideKeyboard", "(Landroid/app/NativeActivity;)V");
            jobject activity = jni.GetInstance();
            jni->CallObjectMethod(shim_class, mid, activity);
            kbVisible = 0;
        }

        bool AShim::KeyboardVisible()
        {
            return kbVisible;
        }

        int AShim::ShowMessageBox(const String& message, const String& caption, int buttons, int hints) 
        {
            //setup for call
            AJniHelper jni;
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

        bool AShim::ConnectedToNetwork()
        {
            //setup for call
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "ConnectedToNetwork", "(Landroid/app/NativeActivity;)Z");
            jobject activity = jni.GetInstance();
            jobject jresult = jni->CallObjectMethod(shim_class, mid, activity);
            bool result = (bool)jresult;
            jni->DeleteLocalRef(jresult);
            jni->DeleteLocalRef(activity);
            return (bool)result;
        }

        // void AShim::SendHttpAsync(const HttpRequest& req)
        // {
        //     AJniHelper jni;
        //     jclass shim_class = jni.GetShim();
        //     jmethodID mid = jni->GetStaticMethodID(shim_class, "SendHttpAsync", "(Landroid/app/NativeActivity;Ljava/lang/String;L[java/lang/String;J)Z");
        //     jobject activity = jni.GetInstance();
        //     if (!mid) LOGE("Cant find async");
        //     //jobject jresult = jni->CallObjectMethod(shim_class, mid, activity);
        // }

        jobject AShim::HttpNewConnection(const String& uri, const String& method, bool hasPayload)
        {
            AJniHelper jni;
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

        void AShim::HttpCloseConnection(jobject httpConnection)
        {
            AJniHelper jni;
            jmethodID mid = jni.GetInstanceMethod(httpConnection, "disconnect", "()V");
            if (!mid) {
                LOGE("Unable to disconnect, cant get mid");
                return; 
            }
            jni->CallObjectMethod(httpConnection, mid);
        }

        AStream* AShim::HttpGetOutputStream(jobject httpConnection)
        {
            AJniHelper jni;
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

        AStream* AShim::HttpGetInputStream(jobject httpConnection)
        {
            AJniHelper jni;
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

        void AShim::HttpSetHeader(jobject httpConnection, const String& key, const String& val)
        {
            AJniHelper jni;
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

        String AShim::HttpGetHeader(jobject httpConnection, const String& key)
        {
            AJniHelper jni;
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

        int AShim::GetResponseCode(jobject httpConnection)
        {
            AJniHelper jni;
            jmethodID mid = jni.GetInstanceMethod(httpConnection, "getResponseCode", "()I");
            if (!mid) LOGE("Unable to get GetResponseCode, cant get mid");
            jobject jobj = jni->CallObjectMethod(httpConnection, mid);
            int result = (int)jobj;
            jni->DeleteLocalRef(jobj);
            return result;
        }

        void AShim::HttpShowHeaders(jobject httpConnection)
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "HttpShowHeaders",
                                                   "(Ljava/net/HttpURLConnection;)V");
            if (!mid) LOGE("Unable to get httpshowheaders, cant get mid");
            jni->CallObjectMethod(shim_class, mid, httpConnection);
        }

        void AShim::InitDefaultCookieManager()
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "InitDefaultCookieManager", "()V");
            if (!mid) LOGE("Unable to get InitDefaultCookieManager, cant get mid");
            jni->CallObjectMethod(shim_class, mid);
        }


        AAssetManager* AShim::GetAssetManager()
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "GetAssetManager", "(Landroid/app/NativeActivity;)Landroid/content/res/AssetManager;");
            if (!mid) LOGE("Unable to GetAssetManager, cant get mid");
            jobject activity = jni.GetInstance();
            jobject assetManager = jni->CallObjectMethod(shim_class, mid, activity);
            jni->NewGlobalRef(assetManager);
            AAssetManager* result = AAssetManager_fromJava(jni.GetEnv(), assetManager);
            return result;
        }

        Xli::Key AShim::AndroidToXliKeyEvent(AKeyEvent androidKeyCode) 
        {
            switch (androidKeyCode)
            {
            case KEYCODE_0:
                return Key0;
            case KEYCODE_1:
                return Key1;
            case KEYCODE_2:
                return Key2;
            case KEYCODE_3:
                return Key3;
            case KEYCODE_4:
                return Key4;
            case KEYCODE_5:
                return Key5;
            case KEYCODE_6:
                return Key6;
            case KEYCODE_7:
                return Key7;
            case KEYCODE_8:
                return Key8;
            case KEYCODE_9:
                return Key9;
            case KEYCODE_A:
                return KeyA;
            case KEYCODE_ALT_LEFT:
                return KeyAlt;
            case KEYCODE_B:
                return KeyB;
            case KEYCODE_BACK:
                return KeyUnknown;
            case KEYCODE_BREAK:
                return KeyBreak;
            case KEYCODE_C:
                return KeyC;
            case KEYCODE_CAPS_LOCK:
                return KeyCapsLock;
            case KEYCODE_CTRL_LEFT:
                return KeyCtrl;
            case KEYCODE_D:
                return KeyD;
            case KEYCODE_DEL:
                return KeyBackspace;
            case KEYCODE_DPAD_DOWN:
                return KeyDown;
            case KEYCODE_DPAD_LEFT:
                return KeyLeft;
            case KEYCODE_DPAD_RIGHT:
                return KeyRight;
            case KEYCODE_DPAD_UP:
                return KeyUp;
            case KEYCODE_E:
                return KeyE;
            case KEYCODE_ENDCALL:
                return KeyEnd;
            case KEYCODE_ENTER:
                return KeyEnter;
            case KEYCODE_ESCAPE:
                return KeyEscape;
            case KEYCODE_F:
                return KeyF;
            case KEYCODE_F1:
                return KeyF1;
            case KEYCODE_F10:
                return KeyF10;
            case KEYCODE_F11:
                return KeyF11;
            case KEYCODE_F12:
                return KeyF12;
            case KEYCODE_F2:
                return KeyF2;
            case KEYCODE_F3:
                return KeyF3;
            case KEYCODE_F4:
                return KeyF4;
            case KEYCODE_F5:
                return KeyF5;
            case KEYCODE_F6:
                return KeyF6;
            case KEYCODE_F7:
                return KeyF7;
            case KEYCODE_F8:
                return KeyF8;
            case KEYCODE_F9:
                return KeyF9;
            case KEYCODE_G:
                return KeyG;
            case KEYCODE_H:
                return KeyH;
            case KEYCODE_HOME:
                return KeyHome;
            case KEYCODE_I:
                return KeyI;
            case KEYCODE_INSERT:
                return KeyInsert;
            case KEYCODE_J:
                return KeyJ;
            case KEYCODE_K:
                return KeyK;
            case KEYCODE_L:
                return KeyL;
            case KEYCODE_M:
                return KeyM;
            case KEYCODE_MENU:
                return KeyMenu;
            case KEYCODE_N:
                return KeyN;
            case KEYCODE_NUMPAD_0:
                return KeyNumPad0;
            case KEYCODE_NUMPAD_1:
                return KeyNumPad1;
            case KEYCODE_NUMPAD_2:
                return KeyNumPad2;
            case KEYCODE_NUMPAD_3:
                return KeyNumPad3;
            case KEYCODE_NUMPAD_4:
                return KeyNumPad4;
            case KEYCODE_NUMPAD_5:
                return KeyNumPad5;
            case KEYCODE_NUMPAD_6:
                return KeyNumPad6;
            case KEYCODE_NUMPAD_7:
                return KeyNumPad7;
            case KEYCODE_NUMPAD_8:
                return KeyNumPad8;
            case KEYCODE_NUMPAD_9:
                return KeyNumPad9;
            case KEYCODE_O:
                return KeyO;
            case KEYCODE_P:
                return KeyP;
            case KEYCODE_PAGE_DOWN:
                return KeyPageDown;
            case KEYCODE_PAGE_UP:
                return KeyPageUp;
            case KEYCODE_Q:
                return KeyQ;
            case KEYCODE_R:
                return KeyR;
            case KEYCODE_S:
                return KeyS;
            case KEYCODE_SHIFT_LEFT:
                return KeyShift;
            case KEYCODE_SPACE:
                return KeySpace;
            case KEYCODE_T:
                return KeyT;
            case KEYCODE_TAB:
                return KeyTab;
            case KEYCODE_U:
                return KeyU;
            case KEYCODE_UNKNOWN:
                return KeyUnknown;
            case KEYCODE_V:
                return KeyV;
            case KEYCODE_W:
                return KeyW;
            case KEYCODE_X:
                return KeyX;
            case KEYCODE_Y:
                return KeyY;
            case KEYCODE_Z:
                return KeyZ; 
            };
            return KeyUnknown;
        }
    }
}
