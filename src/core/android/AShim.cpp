#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <Xli/PlatformSpecific/Android.h>
#include <Xli/Window.h>

//{TODO} All these mids should be cacheable

namespace Xli
{    
    namespace PlatformSpecific
    {
        int AShim::kbVisible = 0;
        bool AShim::midsCached = false;

        void AShim::CacheMids()
        {
            // if (!midsCached)
            // {
            //     makeNoiseMid = ;
            //     raiseSoftKeyboardMid = ;
            //     hideSoftKeyboardMid = ;
            //     keyboardVisibleMid = ;
            //     showMessageBoxMid = ;
            //     connectedToNetworkMid = ;
            //     xliToJavaHeadersMid = ;
            //     sendHttpAsyncMid = ;
            //     sendHttpAsyncMid = ;
            //     sendHttpAsyncMid = ;
            //     abortAsyncConnectionMid = ;
            //     httpNewConnectionMid = ;
            //     inputStreamToStringMid = ;
            //     asyncInputStreamToStringMid = ;
            //     asyncInputStreamToByteArrayMid = ;
            //     readBytesFromInputStreamMid = ;
            //     getAssetManagerMid = ;
            //     registerNativeFunctionsMid = ;
            //     androidToXliKeyEventMid = ;
            //     handleSpecialAndroidKeyEventsMid = ;
            //     initDefaultCookieManagerMid = ;
            // }
        }

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

        jobject AShim::SendHttpAsync(const HttpRequest* req, void* content, long byteLength)
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "SendHttpAsync", "(Landroid/app/NativeActivity;Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/nio/ByteBuffer;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                jobject activity = jni.GetInstance();

                jstring jurl = jni->NewStringUTF(req->Url.Data());
                jstring jmethod = jni->NewStringUTF(HttpMethodToString(req->Method).Data());
                jint jtimeout = (jint)req->Timeout;
                jobject headers = XliToJavaHeaders(&(req->Headers));

                jobject arrayHandle = 0;
                if ((content!=0) && (byteLength>0))
                {
                    arrayHandle = jni->NewDirectByteBuffer(content, byteLength);
                }
            
                jobject jresult = jni->CallObjectMethod(shim_class, mid, activity, 
                                                        jurl, jmethod, headers, arrayHandle,
                                                        jtimeout, (jlong)req);
                jni->DeleteLocalRef(jurl);
                jni->DeleteLocalRef(jmethod);
                jni->DeleteLocalRef(headers);
                return reinterpret_cast<jobject>(jni->NewGlobalRef(jresult));
            } else {
                LOGE("Couldn't find SendHttpAsync");
                return 0;
            }
        }

        jobject AShim::SendHttpAsync(const HttpRequest* req, String content)
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "SendHttpStringAsync", "(Landroid/app/NativeActivity;Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/lang/String;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                jobject activity = jni.GetInstance();

                jstring jurl = jni->NewStringUTF(req->Url.Data());
                jstring jmethod = jni->NewStringUTF(HttpMethodToString(req->Method).Data());
                jint jtimeout = (jint)req->Timeout;
                jobject headers = XliToJavaHeaders(&(req->Headers));
                jobject body = 0;

                if ((content.Length()>0))
                {
                    body = jni->NewStringUTF(content.Data());
                }
            
                jobject jresult = jni->CallObjectMethod(shim_class, mid, activity, 
                                                        jurl, jmethod, headers, body,
                                                        jtimeout, (jlong)req);
                jni->DeleteLocalRef(jurl);
                jni->DeleteLocalRef(jmethod);
                jni->DeleteLocalRef(headers);
                if (body!=0) jni->DeleteLocalRef(body);
                return reinterpret_cast<jobject>(jni->NewGlobalRef(jresult));
            } else {
                LOGE("Couldn't find SendHttpAsync");
                return 0;
            }
        }

        jobject AShim::SendHttpAsync(const HttpRequest* req)
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "SendHttpAsync", "(Landroid/app/NativeActivity;Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/nio/ByteBuffer;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                jobject activity = jni.GetInstance();

                jstring jurl = jni->NewStringUTF(req->Url.Data());
                jstring jmethod = jni->NewStringUTF(HttpMethodToString(req->Method).Data());
                jint jtimeout = (jint)req->Timeout;
                jobject headers = XliToJavaHeaders(&(req->Headers));
                jobject arrayHandle = 0;                
            
                jobject jresult = jni->CallObjectMethod(shim_class, mid, activity, 
                                                        jurl, jmethod, headers, arrayHandle,
                                                        jtimeout, (jlong)req);
                jni->DeleteLocalRef(jurl);
                jni->DeleteLocalRef(jmethod);
                jni->DeleteLocalRef(headers);
                return reinterpret_cast<jobject>(jni->NewGlobalRef(jresult));
            } else {
                LOGE("Couldn't find SendHttpAsync");
                return 0;
            }
        }

        void AShim::AbortAsyncConnection(jobject connection)
        {
            AJniHelper jni;            
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "AbortAsyncConnection", "(Landroid/os/AsyncTask;)V");
            if (!mid) {
                LOGE("Unable to get AbortAsyncConnection mid");
                return;
            }
            jni->CallObjectMethod(shim_class, mid, connection);
        }

        jobject AShim::XliToJavaHeaders(const HashMap<String,String>* src)
        {
            AJniHelper jni;            
            jobject hashmap = jni.GetInstance("java/util/HashMap","()V");
            jmethodID put = jni.GetInstanceMethod(hashmap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

            int i = src->Begin();
            while (i != src->End())
            {
                jstring jkey = jni->NewStringUTF(src->GetKey(i).Data());
                jstring jval = jni->NewStringUTF(src->GetValue(i).Data());
            
                jni->CallObjectMethod(hashmap, put, jkey, jval);
            
                jni->DeleteLocalRef(jkey);
                jni->DeleteLocalRef(jval);
                i = src->Next(i);
            }

            return hashmap;
        }

        String AShim::InputStreamToString(jobject bufferedInputStream)
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "InputStreamToString", "(Ljava/io/InputStream;)Ljava/lang/String;");
            if (!mid) {
                LOGE("Unable to get InputStreamToString mid");
                return "";
            }
            jstring jresult = (jstring)jni->CallObjectMethod(shim_class, mid, bufferedInputStream,(jint)1024);
            String result = jni.GetString(jresult);
            jni->DeleteLocalRef(jresult);
            return result;
        }

        jobject AShim::AsyncInputStreamToString(jobject bufferedInputStream, HttpRequest* request)
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "AsyncInputStreamToString", "(Ljava/io/InputStream;J)Landroid/os/AsyncTask;");
            if (!mid) {
                LOGE("Unable to get AsyncInputStreamToString mid");
                return 0;
            }
            return jni->CallObjectMethod(shim_class, mid, bufferedInputStream, (long)request);
        }

        jobject AShim::AsyncInputStreamToByteArray(jobject bufferedInputStream, HttpRequest* request)
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "AsyncInputStreamToByteArray", "(Ljava/io/InputStream;J)Landroid/os/AsyncTask;");
            if (!mid) {
                LOGE("Unable to get AsyncInputStreamToStringByteArray mid");
                return 0;
            }
            return jni->CallObjectMethod(shim_class, mid, bufferedInputStream, (long)request);
        }

        int AShim::ReadBytesFromInputStream(jobject bufferedInputStream, int bytesToRead, void* dst)
        {
            AJniHelper jni;
            jclass shim_class = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shim_class, "ReadBytesFromInputStream", "(Ljava/io/BufferedInputStream;I)[B");
            if (!mid) {
                LOGE("Unable to get ReadBytesFromInputStream mid");
                return 0;
            }
            jbyteArray jbytes = (jbyteArray)jni->CallObjectMethod(shim_class, mid, bufferedInputStream, (jint)bytesToRead);
            if (jbytes != 0) {
                jsize len = jni->GetArrayLength(jbytes);
                jni->GetByteArrayRegion(jbytes, 0, len, (jbyte*)dst);
                int result = (int)len;
                jni->DeleteLocalRef(jbytes);
                //jni->DeleteLocalRef((jobject)len);
                return result;
            } else {
                return -1;
            }
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
            assetManager = reinterpret_cast<jobject>(jni->NewGlobalRef(assetManager));
            AAssetManager* result = AAssetManager_fromJava(jni.GetEnv(), assetManager);
            return result;
        }

        bool AShim::RegisterNativeFunctions(JNINativeMethod native_funcs[], int funcCount)
        {
            AJniHelper jni;            
            jclass shim_class = jni.GetShim();
            jint attached = jni->RegisterNatives(shim_class, native_funcs, (jint)funcCount);
            return (attached >= 0);
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
