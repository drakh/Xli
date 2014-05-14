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
        jmethodID AShim::hasVibrator;
        jmethodID AShim::vibrateForMilliseconds;

        void AShim::CacheMids(JNIEnv *env, jclass shimClass)
        {
            //LOGD("in_0");
            LOGD("Caching Mids");
            raiseKeyboard = env->GetStaticMethodID(shimClass, "raiseKeyboard", "()V");
            hideKeyboard = env->GetStaticMethodID(shimClass, "hideKeyboard", "()V");
            getKeyboardSize = env->GetStaticMethodID(shimClass, "GetKeyboardSize", "()I");
            showMessageBox = env->GetStaticMethodID(shimClass, "ShowMessageBox", "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;II)I");
            connectedToNetwork = env->GetStaticMethodID(shimClass, "ConnectedToNetwork", "()Z");
            initDefaultCookieManager = env->GetStaticMethodID(shimClass, "InitDefaultCookieManager", "()V");
            getAssetManager = env->GetStaticMethodID(shimClass, "GetAssetManager", "()Landroid/content/res/AssetManager;");
            hideStatusBar = env->GetStaticMethodID(shimClass, "hideStatusBar", "()V");
            showStatusBar = env->GetStaticMethodID(shimClass, "showStatusBar", "()V");
            getStatusBarHeight = env->GetStaticMethodID(shimClass, "GetStatusBarHeight", "()F");
            getDisplayMetrics = env->GetStaticMethodID(shimClass, "GetDisplayMetrics", "()Landroid/util/DisplayMetrics;");

            hasVibrator = env->GetStaticMethodID(shimClass, "HasVibrator", "()Z");
            vibrateForMilliseconds = env->GetStaticMethodID(shimClass, "VibrateForMilliseconds", "(I)V");

            if (!raiseKeyboard) XLI_THROW("Cannot cache mid for raiseKeyboard.");
            if (!hideKeyboard) XLI_THROW("Cannot cache mid for hideKeyboard.");
            if (!getKeyboardSize) XLI_THROW("Cannot cache mid for getKeyboardSize.");
            if (!showMessageBox) XLI_THROW("Cannot cache mid for showMessageBox.");
            if (!connectedToNetwork) XLI_THROW("Cannot cache mid for connectedToNetwork.");
            if (!initDefaultCookieManager) XLI_THROW("Cannot cache mid for initDefaultCookieManager.");
            if (!getAssetManager) XLI_THROW("Cannot cache mid for getAssetManager.");
            if (!hideStatusBar) XLI_THROW("Cannot cache mid for hideStatusBar.");
            if (!showStatusBar) XLI_THROW("Cannot cache mid for showStatusBar.");
            if (!getStatusBarHeight) XLI_THROW("Cannot cache mid for getStatusBarHeight.");
            if (!getDisplayMetrics) XLI_THROW("Cannot cache mid for getDisplayMetrics.");
            if (!hasVibrator) XLI_THROW("Cannot cache mid for hasVibrator.");
            if (!vibrateForMilliseconds) XLI_THROW("Cannot cache mid for vibrateForMilliseconds.");            
            LOGD("Mids Cached");
            //LOGD("out_0");
        }

        void AShim::RaiseSoftKeyboard()
        {
            //LOGD("in_1");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallObjectMethod(shimClass, raiseKeyboard);
            kbVisible = 1;
            //LOGD("out_2");
        }

        void AShim::HideSoftKeyboard()
        {
            //LOGD("in_3");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallObjectMethod(shimClass, hideKeyboard);
            kbVisible = 0;
            //LOGD("out_3");
        }

        int AShim::GetKeyboardSize()
        {
            //LOGD("in_4");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jint result = (jint)jni->CallStaticIntMethod(shimClass, getKeyboardSize);
            //LOGD("out_4");
            return (int)result;
        }

        void AShim::HideStatusBar()
        {
            //LOGD("in_5");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallObjectMethod(shimClass, hideStatusBar);
            //{TODO} When this is working, go fix AWindow statusbar methods
            //LOGD("out_5");
        }

        void AShim::ShowStatusBar()
        {
            //LOGD("in_6");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallObjectMethod(shimClass, showStatusBar);
            //{TODO} When this is working, go fix AWindow statusbar methods
            //LOGD("out_6");
        }

        float AShim::GetStatusBarHeight()
        {
            //LOGD("in_7");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jfloat height = jni->CallStaticFloatMethod(shimClass, getStatusBarHeight);
            //LOGD("out_7");
            return (float)height;            
        }

        float AShim::GetDensity()
        {
            //LOGD("in_8");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jclass metricsClass = jni->FindClass("android/util/DisplayMetrics");
            jfieldID jd = jni->GetFieldID( metricsClass, "density", "F" );
            jobject result = jni->CallStaticObjectMethod(shimClass, getDisplayMetrics);
            jfloat d = (jfloat)jni->GetFloatField(result, jd);
            //LOGD("out_8");
            return (float)d;
        }

        Vector2 AShim::GetDpi()
        {
            //LOGD("in_9");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jclass metricsClass = jni->FindClass("android/util/DisplayMetrics");
            jfieldID xDpi = jni->GetFieldID( metricsClass, "xdpi", "F" );
            jfieldID yDpi = jni->GetFieldID( metricsClass, "ydpi", "F" );
            jobject result = jni->CallStaticObjectMethod(shimClass, getDisplayMetrics);
            jfloat jx = (jfloat)jni->GetFloatField(result, xDpi);
            jfloat jy = (jfloat)jni->GetFloatField(result, yDpi);
            //LOGD("out_9");
            return Vector2((float)jx, (float)jy);
        }

        bool AShim::KeyboardVisible()
        {
            //LOGD("in_10");
            return kbVisible;
            //LOGD("out_10");
        }

        bool AShim::HasVibrator()
        {
            //LOGD("in_11");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject jresult = jni->CallObjectMethod(shimClass, hasVibrator);
            bool result = (bool)jresult;
            jni->DeleteLocalRef(jresult);
            //LOGD("out_11");
            return (bool)result;
        }

        void AShim::VibrateForMilliseconds(int milliseconds)
        {
            //LOGD("in_12");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallObjectMethod(shimClass, vibrateForMilliseconds, (jint)milliseconds);
            //LOGD("out_12");
        }

        int AShim::ShowMessageBox(const String& message, const String& caption, int buttons, int hints)
        {
            //LOGD("in_13");
            //setup for call
            AJniHelper jni;
            jclass shimClass = jni.GetShim();

            //vars for call
            jstring jcaption = jni->NewStringUTF(caption.DataPtr());
            jstring jmessage = jni->NewStringUTF(message.DataPtr());

            //call
            int result = (int)jni->CallObjectMethod(shimClass, showMessageBox, jcaption, jmessage, (jint)buttons, (jint)hints);
            //LOGD("out_13");
            return result;
        }

        bool AShim::ConnectedToNetwork()
        {
            //LOGD("in_14");
            //setup for call
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject jresult = jni->CallObjectMethod(shimClass, connectedToNetwork);
            bool result = (bool)jresult;
            jni->DeleteLocalRef(jresult);
            //LOGD("out_14");
            return (bool)result;
        }

        jobject AShim::SendHttpAsync(const HttpRequest* req, const void* content, long byteLength)
        {
            //LOGD("in_15");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "SendHttpAsync", "(Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/nio/ByteBuffer;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                String url = req->GetUrl();
                String method = req->GetMethod();

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

                jobject jresult = jni->CallObjectMethod(shimClass, mid, jurl, jmethod, headers, arrayHandle,
                                                        jtimeout, (jlong)req);
                jni->DeleteLocalRef(jurl);
                jni->DeleteLocalRef(jmethod);
                jni->DeleteLocalRef(headers);
                if (jresult==0)
                {
                    LOGE("AShim [SendHttpAsync - string]: Could not create async http request");
                    //LOGD("out_15");
                    return 0;
                } else {
                    //LOGD("out_15");
                    return reinterpret_cast<jobject>(jni->NewGlobalRef(jresult));
                }
            } else {
                LOGE("Couldn't find SendHttpAsync");
                //LOGD("out_15");
                return 0;
            }
            //LOGD("out_15");
        }

        jobject AShim::SendHttpAsync(const HttpRequest* req, String content)
        {
            //LOGD("in_16");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "SendHttpStringAsync", "(Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/lang/String;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                String url = req->GetUrl();
                String method = req->GetMethod();

                jstring jurl = jni->NewStringUTF(url.DataPtr());
                jstring jmethod = jni->NewStringUTF(method.DataPtr());
                jint jtimeout = (jint)req->GetTimeout();
                jobject headers = XliToJavaHeaders(req);
                jobject body = 0;

                if ((content.Length()>0))
                {
                    body = jni->NewStringUTF(content.DataPtr());
                }

                jobject jresult = jni->CallObjectMethod(shimClass, mid, jurl, jmethod, headers, body,
                                                        jtimeout, (jlong)req);
                jni->DeleteLocalRef(jurl);
                jni->DeleteLocalRef(jmethod);
                jni->DeleteLocalRef(headers);
                if (body!=0) jni->DeleteLocalRef(body);

                if (jresult==0)
                {
                    LOGE("AShim [SendHttpAsync - string]: Could not create async http request");
                    //LOGD("out_16");
                    return 0;
                } else {
                    //LOGD("out_16");
                    return reinterpret_cast<jobject>(jni->NewGlobalRef(jresult));
                }
            } else {
                LOGE("Couldn't find SendHttpAsync");
                //LOGD("out_16");
                return 0;
            }
            //LOGD("out_16");
        }

        jobject AShim::SendHttpAsync(const HttpRequest* req)
        {
            //LOGD("in_17");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();

            jmethodID mid = jni->GetStaticMethodID(shimClass, "SendHttpAsync", "(Ljava/lang/String;Ljava/lang/String;Ljava/util/HashMap;Ljava/nio/ByteBuffer;IJ)Landroid/os/AsyncTask;");
            if (mid)
            {
                String url = req->GetUrl();
                String method = req->GetMethod();

                jstring jurl = jni->NewStringUTF(url.DataPtr());
                jstring jmethod = jni->NewStringUTF(method.DataPtr());
                jint jtimeout = (jint)req->GetTimeout();
                jobject headers = XliToJavaHeaders(req);
                jobject arrayHandle = 0;

                jobject jresult = jni->CallObjectMethod(shimClass, mid, jurl, jmethod, headers, arrayHandle,
                                                        jtimeout, (jlong)req);
                jni->DeleteLocalRef(jurl);
                jni->DeleteLocalRef(jmethod);
                jni->DeleteLocalRef(headers);
                if (jresult==0)
                {
                    LOGE("AShim [SendHttpAsync - string]: Could not create async http request");
                    //LOGD("out_17");
                    return 0;
                } else {
                    //LOGD("out_17");
                    return reinterpret_cast<jobject>(jni->NewGlobalRef(jresult));
                }
            } else {
                LOGE("Couldn't find SendHttpAsync");
                //LOGD("out_17");
                return 0;
            }
            //LOGD("out_17");
        }

        void AShim::AbortAsyncConnection(jobject connection)
        {
            //LOGD("in_18");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "AbortAsyncConnection", "(Landroid/os/AsyncTask;)V");
            if (!mid) {
                LOGE("Unable to get AbortAsyncConnection mid");
                //LOGD("out_18");
                return;
            }
            jni->CallObjectMethod(shimClass, mid, connection);
            //LOGD("out_18");
        }

        jobject AShim::XliToJavaHeaders(const HttpRequest* req)
        {
            //LOGD("in_19");
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

            //LOGD("out_19");
            return hashmap;
        }

        String AShim::InputStreamToString(jobject bufferedInputStream)
        {
            //LOGD("in_20");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "InputStreamToString", "(Ljava/io/InputStream;)Ljava/lang/String;");
            if (!mid) {
                LOGE("Unable to get InputStreamToString mid");
                //LOGD("out_20");
                return "";
            }
            jstring jresult = (jstring)jni->CallObjectMethod(shimClass, mid, bufferedInputStream,(jint)1024);
            String result = jni.GetString(jresult);
            jni->DeleteLocalRef(jresult);
            //LOGD("out_20");
            return result;            
        }

        jobject AShim::AsyncInputStreamToString(jobject bufferedInputStream, HttpRequest* request)
        {
            //LOGD("in_21");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "AsyncInputStreamToString", "(Ljava/io/InputStream;J)Landroid/os/AsyncTask;");
            if (!mid) {
                LOGE("Unable to get AsyncInputStreamToString mid");
                //LOGD("out_21");
                return 0;
            }
            //LOGD("out_21");
            return jni->CallObjectMethod(shimClass, mid, bufferedInputStream, (long)request);
        }

        jobject AShim::AsyncInputStreamToByteArray(jobject bufferedInputStream, HttpRequest* request)
        {
            //LOGD("in_22");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jmethodID mid = jni->GetStaticMethodID(shimClass, "AsyncInputStreamToByteArray", "(Ljava/io/InputStream;J)Landroid/os/AsyncTask;");
            if (!mid) {
                LOGE("Unable to get AsyncInputStreamToStringByteArray mid");
                //LOGD("out_22");
                return 0;
            }
            //LOGD("out_22");
            return jni->CallObjectMethod(shimClass, mid, bufferedInputStream, (long)request);
        }

        int AShim::ReadBytesFromInputStream(jobject bufferedInputStream, int bytesToRead, void* dst)
        {
            //LOGD("in_23");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();

            jmethodID mid = jni->GetStaticMethodID(shimClass, "ReadBytesFromInputStream", "(Ljava/io/BufferedInputStream;I)[B");
            if (!mid) {
                LOGE("Unable to get ReadBytesFromInputStream mid");
                //LOGD("out_23");
                return 0;
            }
            jbyteArray jbytes = (jbyteArray)jni->CallObjectMethod(shimClass, mid, bufferedInputStream, (jint)bytesToRead);
            if (jbytes != 0) {
                jsize len = jni->GetArrayLength(jbytes);
                jni->GetByteArrayRegion(jbytes, 0, len, (jbyte*)dst);
                int result = (int)len;
                jni->DeleteLocalRef(jbytes);
                //jni->DeleteLocalRef((jobject)len);
                //LOGD("out_23");
                return result;
            } else {
                //LOGD("out_23");
                return -1;
            }
            //LOGD("out_23");
        }

        void AShim::InitDefaultCookieManager()
        {
            //LOGD("in_24");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jni->CallObjectMethod(shimClass, initDefaultCookieManager);
            //LOGD("out_24");
        }


        AAssetManager* AShim::GetAssetManager()
        {
            //LOGD("in_25");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jobject assetManager = jni->CallObjectMethod(shimClass, getAssetManager);
            jni->NewGlobalRef(assetManager);
            AAssetManager* result = AAssetManager_fromJava(jni.GetEnv(), assetManager);
            //LOGD("out_25");
            return result;
        }

        bool AShim::RegisterNativeFunctions(JNINativeMethod native_funcs[], int funcCount)
        {
            //LOGD("in_26");
            AJniHelper jni;
            jclass shimClass = jni.GetShim();
            jint attached = jni->RegisterNatives(shimClass, native_funcs, (jint)funcCount);
            //LOGD("out_26");
            return (attached >= 0);
        }        
    }
}
