#ifndef __XLI_ASHIM_ANDROID_H__
#define __XLI_ASHIM_ANDROID_H__

#include <jni.h>
#include <Xli/String.h>
#include <XliHttpClient.h>
#include "AInternal.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class AShim
        {
        private:
            static jmethodID onPause;
            static jmethodID onResume;

            static jmethodID raiseKeyboard;
            static jmethodID hideKeyboard;
            static jmethodID getKeyboardSize;
            static jmethodID showMessageBox;
            static jmethodID connectedToNetwork;
            static jmethodID newHttpConnection;
            static jmethodID httpGetOutputStream;
            static jmethodID httpGetInputStream;
            static jmethodID httpShowHeaders;
            static jmethodID initDefaultCookieManager;
            static jmethodID getAssetManager;
            static jmethodID hideStatusBar;
            static jmethodID showStatusBar;
            static jmethodID getStatusBarHeight;
            static jmethodID getDisplayMetrics;
            static jmethodID hasVibrator;
            static jmethodID vibrateForMilliseconds;
            static jmethodID abortAsyncTask;
            static jmethodID holdObject;
            static jmethodID getObject;
            static jmethodID tryReleaseObject;
            static jmethodID asyncInputStreamToString;
            static jmethodID asyncInputStreamToByteArray;
            static jmethodID sendHttpAsyncA;
            static jmethodID sendHttpAsyncB;
            static jmethodID getHeaderMap;
            
        public:
            static int kbVisible;

            static void CacheMids(JNIEnv *env, jclass shim_class);
            static void OnPause();
            static void OnResume();
            static void RaiseSoftKeyboard();
            static void HideSoftKeyboard();
            static bool KeyboardVisible();
            static int GetKeyboardSize();
            static void ShowStatusBar();
            static void HideStatusBar();
            static float GetStatusBarHeight();
            static int ShowMessageBox(const String& message, const String& caption, int buttons, int hints);
            static bool HasVibrator();
            static void VibrateForMilliseconds(int milliseconds);
            
            static bool ConnectedToNetwork();
            static String HeadersToString(const HttpRequest* req);
            static JObjRef SendHttpAsync(const HttpRequest* req, const void* content, long byteLength);
            static JObjRef SendHttpAsync(const HttpRequest* req, String content);
            static JObjRef SendHttpAsync(const HttpRequest* req);
            static void AbortAsyncTask(JObjRef task);
            static String InputStreamToString(jobject bufferedInputStream);
            static JObjRef AsyncInputStreamToString(JObjRef bufferedInputStream, HttpRequest* request);
            static JObjRef AsyncInputStreamToByteArray(JObjRef bufferedInputStream, HttpRequest* request);
            static int ReadBytesFromInputStream(jobject bufferedInputStream, int bytesToRead, void* dst);
            static AAssetManager* GetAssetManager();
            static bool RegisterNativeFunctions(JNINativeMethod native_funcs[], int funcCount);
            static void InitDefaultCookieManager();
            static float GetDensity();
            static Vector2 GetDpi();
            static JObjRef HoldObject(jobject obj);
            static jobject GetObject(JObjRef objKey);
            static bool TryReleaseObject(JObjRef objKey);
            static jobject GetHeaderMap();
        };
    };
};

#endif
