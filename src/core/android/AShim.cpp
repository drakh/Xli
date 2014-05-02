#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <Xli/PlatformSpecific/Android.h>
#include "AShim.h"
#include "AJniHelper.h"
#include <Xli/Window.h>

//{TODO} All these mids should be cacheable

namespace Xli
{
    namespace PlatformSpecific
    {
        int AShim::kbVisible = 0;

        jmethodID AShim::makeNoise;
        jmethodID AShim::raiseKeyboard;
        jmethodID AShim::hideKeyboard;
        jmethodID AShim::getKeyboardSize;
        jmethodID AShim::showMessageBox;
        jmethodID AShim::connectedToNetwork;
        jmethodID AShim::httpShowHeaders;
        jmethodID AShim::initDefaultCookieManager;
        jmethodID AShim::getAssetManager;
        jmethodID AShim::hideStatusBar;
        jmethodID AShim::showStatusBar;
        jmethodID AShim::getStatusBarHeight;
        jmethodID AShim::getDisplayMetrics;

        void AShim::CacheMids(JNIEnv *env, jclass shimClass)
        {
            LOGE("Caching Mids");
            makeNoise = env->GetStaticMethodID(shimClass, "makeNoise", "()V");
            raiseKeyboard = env->GetStaticMethodID(shimClass, "raiseKeyboard", "(Landroid/app/NativeActivity;)V");
            hideKeyboard = env->GetStaticMethodID(shimClass, "hideKeyboard", "(Landroid/app/NativeActivity;)V");
            getKeyboardSize = env->GetStaticMethodID(shimClass, "GetKeyboardSize", "()I");
            showMessageBox = env->GetStaticMethodID(shimClass, "ShowMessageBox", "(Landroid/app/NativeActivity;Ljava/lang/CharSequence;Ljava/lang/CharSequence;II)I");
            connectedToNetwork = env->GetStaticMethodID(shimClass, "ConnectedToNetwork", "(Landroid/app/NativeActivity;)Z");
            initDefaultCookieManager = env->GetStaticMethodID(shimClass, "InitDefaultCookieManager", "()V");
            getAssetManager = env->GetStaticMethodID(shimClass, "GetAssetManager", "(Landroid/app/NativeActivity;)Landroid/content/res/AssetManager;");
            hideStatusBar = env->GetStaticMethodID(shimClass, "hideStatusBar", "(Landroid/app/NativeActivity;)V");
            showStatusBar = env->GetStaticMethodID(shimClass, "showStatusBar", "(Landroid/app/NativeActivity;)V");
            getStatusBarHeight = env->GetStaticMethodID(shimClass, "GetStatusBarHeight", "(Landroid/app/NativeActivity;)F");
            getDisplayMetrics = env->GetStaticMethodID(shimClass, "GetDisplayMetrics", "(Landroid/app/NativeActivity;)Landroid/util/DisplayMetrics;");

            if (!makeNoise) {
                XLI_THROW("Cannot cache mid for makeNoise.");
            }
            if (!raiseKeyboard) {
                XLI_THROW("Cannot cache mid for raiseKeyboard.");
            }
            if (!hideKeyboard) {
                XLI_THROW("Cannot cache mid for hideKeyboard.");
            }
            if (!getKeyboardSize) {
                XLI_THROW("Cannot cache mid for getKeyboardSize.");
            }
            if (!showMessageBox) {
                XLI_THROW("Cannot cache mid for showMessageBox.");
            }
            if (!connectedToNetwork) {
                XLI_THROW("Cannot cache mid for connectedToNetwork.");
            }
            if (!initDefaultCookieManager) {
                XLI_THROW("Cannot cache mid for initDefaultCookieManager.");
            }
            if (!getAssetManager) {
                XLI_THROW("Cannot cache mid for getAssetManager.");
            }
            if (!hideStatusBar) {
                XLI_THROW("Cannot cache mid for hideStatusBar.");
            }
            if (!showStatusBar) {
                XLI_THROW("Cannot cache mid for showStatusBar.");
            }
            if (!getStatusBarHeight) {
                XLI_THROW("Cannot cache mid for getStatusBarHeight.");
            }
            if (!getDisplayMetrics) {
                XLI_THROW("Cannot cache mid for getDisplayMetrics.");
            }
            if ((!makeNoise) || (!raiseKeyboard) || (!hideKeyboard) || (!showMessageBox) || (!connectedToNetwork) || 
                (!initDefaultCookieManager) || (!getAssetManager) || (!getKeyboardSize) || (!showStatusBar) ||
                (!hideStatusBar)  || (!getDisplayMetrics) || (!getStatusBarHeight))
            {
                XLI_THROW("Cannot cache mids for shim. Exiting.");
            }
            LOGE("Mids Cached");
        }

        void AShim::MakeNoise()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallObjectMethod(shimClass, makeNoise);
        }

        void AShim::RaiseSoftKeyboard()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jni->CallObjectMethod(shimClass, raiseKeyboard, activity);
            kbVisible = 1;
        }

        void AShim::HideSoftKeyboard()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jni->CallObjectMethod(shimClass, hideKeyboard, activity);
            kbVisible = 0;
        }

        int AShim::GetKeyboardSize()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jint result = (jint)jni->CallStaticIntMethod(shimClass, getKeyboardSize);
            return (int)result;
        }

        void AShim::HideStatusBar()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jni->CallObjectMethod(shimClass, hideStatusBar, activity);
            //{TODO} When this is working, go fix AWindow statusbar methods
        }

        void AShim::ShowStatusBar()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jni->CallObjectMethod(shimClass, showStatusBar, activity);
            //{TODO} When this is working, go fix AWindow statusbar methods
        }

        float AShim::GetStatusBarHeight()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jfloat height = jni->CallStaticFloatMethod(shimClass, getStatusBarHeight, activity);
            return (float)height;            
        }

        float AShim::GetDensity()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jclass metricsClass = jni->FindClass("android/util/DisplayMetrics");
            jfieldID jd = jni->GetFieldID( metricsClass, "density", "F" );
            jobject result = jni->CallStaticObjectMethod(shimClass, getDisplayMetrics, activity);
            jfloat d = (jfloat)jni->GetFloatField(result, jd);
            return (float)d;
        }

        Vector2 AShim::GetDpi()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jclass metricsClass = jni->FindClass("android/util/DisplayMetrics");
            jfieldID xDpi = jni->GetFieldID( metricsClass, "xdpi", "F" );
            jfieldID yDpi = jni->GetFieldID( metricsClass, "ydpi", "F" );
            jobject result = jni->CallStaticObjectMethod(shimClass, getDisplayMetrics, activity);
            jfloat jx = (jfloat)jni->GetFloatField(result, xDpi);
            jfloat jy = (jfloat)jni->GetFloatField(result, yDpi);
            return Vector2((float)jx, (float)jy);
        }

        bool AShim::KeyboardVisible()
        {
            return kbVisible;
        }

        int AShim::ShowMessageBox(const String& message, const String& caption, int buttons, int hints)
        {
            //setup for call
            AJniHelper jni;
            jclass shimClass = jni.GetShim();

            //vars for call
            jobject activity = jni.GetInstance();
            jstring jcaption = jni->NewStringUTF(caption.DataPtr());
            jstring jmessage = jni->NewStringUTF(message.DataPtr());

            //call
            int result = (int)jni->CallObjectMethod(shimClass, showMessageBox, activity, jmessage, jcaption, (jint)buttons, (jint)hints);
            return result;
        }

        bool AShim::ConnectedToNetwork()
        {
            //setup for call
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jobject jresult = jni->CallObjectMethod(shimClass, connectedToNetwork, activity);
            bool result = (bool)jresult;
            jni->DeleteLocalRef(jresult);
            jni->DeleteLocalRef(activity);
            return (bool)result;
        }

        jobject AShim::SendHttpAsync(const HttpRequest* req, const void* content, long byteLength)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "SendHttpAsync", "(Landroid/app/NativeActivity;Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/nio/ByteBuffer;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                String url = req->GetUrl();
                String method = req->GetMethod();

                jobject activity = jni.GetInstance();
                
                jstring jurl = jni->NewStringUTF(url.DataPtr());
                jstring jmethod = jni->NewStringUTF(method.DataPtr());
                jint jtimeout = (jint)req->GetTimeout();
                jobject headers = XliToJavaHeaders(req);

                jobject arrayHandle = 0;
                if ((content!=0) && (byteLength>0))
                {
                    // no way around the const_cast here, we dont modify the data in java so should be safe.
                    arrayHandle = jni->NewDirectByteBuffer(const_cast<void*>(content), byteLength);
                }

                jobject jresult = jni->CallObjectMethod(shimClass, mid, activity,
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
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "SendHttpStringAsync", "(Landroid/app/NativeActivity;Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/lang/String;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                String url = req->GetUrl();
                String method = req->GetMethod();

                jobject activity = jni.GetInstance();

                jstring jurl = jni->NewStringUTF(url.DataPtr());
                jstring jmethod = jni->NewStringUTF(method.DataPtr());
                jint jtimeout = (jint)req->GetTimeout();
                jobject headers = XliToJavaHeaders(req);
                jobject body = 0;

                if ((content.Length()>0))
                {
                    body = jni->NewStringUTF(content.DataPtr());
                }

                jobject jresult = jni->CallObjectMethod(shimClass, mid, activity,
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
            jclass shimClass = jni.GetShim();

            jmethodID mid = jni->GetStaticMethodID(shimClass, "SendHttpAsync", "(Landroid/app/NativeActivity;Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/nio/ByteBuffer;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                String url = req->GetUrl();
                String method = req->GetMethod();

                jobject activity = jni.GetInstance();

                jstring jurl = jni->NewStringUTF(url.DataPtr());
                jstring jmethod = jni->NewStringUTF(method.DataPtr());
                jint jtimeout = (jint)req->GetTimeout();
                jobject headers = XliToJavaHeaders(req);
                jobject arrayHandle = 0;

                jobject jresult = jni->CallObjectMethod(shimClass, mid, activity,
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
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "AbortAsyncConnection", "(Landroid/os/AsyncTask;)V");
            if (!mid) {
                LOGE("Unable to get AbortAsyncConnection mid");
                return;
            }
            jni->CallObjectMethod(shimClass, mid, connection);
        }

        jobject AShim::XliToJavaHeaders(const HttpRequest* req)
        {
            AJniHelper jni;
            jobject hashmap = jni.GetInstance("java/util/HashMap","()V");
            jmethodID put = jni.GetInstanceMethod(hashmap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

            int i = req->HeadersBegin();
            while (i != req->HeadersEnd())
            {
                jstring jkey = jni->NewStringUTF(req->GetHeaderKey(i).DataPtr());
                jstring jval = jni->NewStringUTF(req->GetHeaderValue(i).DataPtr());

                jni->CallObjectMethod(hashmap, put, jkey, jval);

                jni->DeleteLocalRef(jkey);
                jni->DeleteLocalRef(jval);
                i = req->HeadersNext(i);
            }

            return hashmap;
        }

        String AShim::InputStreamToString(jobject bufferedInputStream)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "InputStreamToString", "(Ljava/io/InputStream;)Ljava/lang/String;");
            if (!mid) {
                LOGE("Unable to get InputStreamToString mid");
                return "";
            }
            jstring jresult = (jstring)jni->CallObjectMethod(shimClass, mid, bufferedInputStream,(jint)1024);
            String result = jni.GetString(jresult);
            jni->DeleteLocalRef(jresult);
            return result;
        }

        jobject AShim::AsyncInputStreamToString(jobject bufferedInputStream, HttpRequest* request)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "AsyncInputStreamToString", "(Ljava/io/InputStream;J)Landroid/os/AsyncTask;");
            if (!mid) {
                LOGE("Unable to get AsyncInputStreamToString mid");
                return 0;
            }
            return jni->CallObjectMethod(shimClass, mid, bufferedInputStream, (long)request);
        }

        jobject AShim::AsyncInputStreamToByteArray(jobject bufferedInputStream, HttpRequest* request)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "AsyncInputStreamToByteArray", "(Ljava/io/InputStream;J)Landroid/os/AsyncTask;");
            if (!mid) {
                LOGE("Unable to get AsyncInputStreamToStringByteArray mid");
                return 0;
            }
            return jni->CallObjectMethod(shimClass, mid, bufferedInputStream, (long)request);
        }

        int AShim::ReadBytesFromInputStream(jobject bufferedInputStream, int bytesToRead, void* dst)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();

            jmethodID mid = jni->GetStaticMethodID(shimClass, "ReadBytesFromInputStream", "(Ljava/io/BufferedInputStream;I)[B");
            if (!mid) {
                LOGE("Unable to get ReadBytesFromInputStream mid");
                return 0;
            }
            jbyteArray jbytes = (jbyteArray)jni->CallObjectMethod(shimClass, mid, bufferedInputStream, (jint)bytesToRead);
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
            jclass shimClass = jni.GetShim();
            jni->CallObjectMethod(shimClass, initDefaultCookieManager);
        }


        AAssetManager* AShim::GetAssetManager()
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject activity = jni.GetInstance();
            jobject assetManager = jni->CallObjectMethod(shimClass, getAssetManager, activity);
            jni->NewGlobalRef(assetManager);
            AAssetManager* result = AAssetManager_fromJava(jni.GetEnv(), assetManager);
            return result;
        }

        bool AShim::RegisterNativeFunctions(JNINativeMethod native_funcs[], int funcCount)
        {
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jint attached = jni->RegisterNatives(shimClass, native_funcs, (jint)funcCount);
            return (attached >= 0);
        }        
    }
}
