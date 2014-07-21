#ifndef __XLI_AJNI_ANDROID_H__
#define __XLI_AJNI_ANDROID_H__

#include <jni.h>
#include "CTKeyAction.h"
#include "AShim.h"

namespace Xli
{
    namespace PlatformSpecific
    {
        class AJniHelper
        {
            JNIEnv* env;
            jclass shim;
            static int shim_loaded;

        public:
            static void Init();

            AJniHelper();

            jclass GetShim();

            jmethodID FindMethod(const char* className, const char* methodName, const char* methodSig);

            jobject CallObjectMethod(jobject inst, const char* name, const char* sig);

            String GetString(jobject str);
            JNIEnv* GetEnv();

            JNIEnv* operator->();

            jmethodID GetInstanceMethod(const char* m_name, const char* m_sig);
            jmethodID GetInstanceMethod(jobject inst, const char* m_name, const char* m_sig);
            jobject GetInstance();
            jobject GetInstance(const char* class_name, const char* constructor_sig, ...);
            jobject GetInstance(jclass cls, const char* constructor_sig, ...);
            jclass GetCustomClass(const char* class_path);
            int PrepareAssetJar(const char* file_name, const char* class_name);
            int PrepareAssetJar(const char* file_name, const char* class_name, int package);
            jclass GetAssetClass(const char* file_name, const char* class_name);
        };        

        class CTError : public WindowAction
        {
        public:
            String message;
            int errorCode;
            CTError(String message, int errorCode) 
            { 
                this->message = message; 
                this->errorCode = errorCode;
            }
            virtual void Execute()
            {
                String finalMessage = "XLiError (" + String(errorCode)+ ") - " + message;
                XLI_THROW(finalMessage.DataPtr());
            }
        };
    };
};

#endif
