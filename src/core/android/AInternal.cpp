#include "AInternal.h"
#include "AXliJ.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

namespace Xli
{
	struct ANativeActivity* AndroidActivity = 0;
	
	static pthread_key_t JniThreadKey;
    static pthread_key_t JniShimKey;

    static unsigned char shimApkData[] = {
        #include "shimApkData.h"
    };

	static void JniDestroyThread(void* value)
	{
		LOGD("JNI: Detaching current thread");

		JNIEnv* env = (JNIEnv*)value;
		AndroidActivity->vm->DetachCurrentThread();
		pthread_setspecific(JniThreadKey, NULL);
	}

    int JniHelper::shim_loaded = 0;

	static void JniDestroyShim(void* value)
	{
        // where can we DeleteGlobalRef(*shim) ?
		LOGD("JNI: Freeing Shim class");
		jclass* shim = (jclass*)value;
		delete shim;
		pthread_setspecific(JniShimKey, NULL);
	}

    extern "C"
    {

    }

	void JniHelper::Init()
	{
		if (pthread_key_create(&JniThreadKey, JniDestroyThread))
			LOGE("JNI ERROR: Unable to create pthread key"); // Not fatal
		if (pthread_key_create(&JniShimKey, JniDestroyShim))
			LOGE("JNI ERROR: Unable to Shim pthread key"); // Not fatal
	}

	JniHelper::JniHelper()
	{
		if (AndroidActivity->vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
		{
			if (AndroidActivity->vm->AttachCurrentThread(&env, NULL) != JNI_OK)
            {
				XLI_THROW("JNI ERROR: Failed to attach current thread");
            } else {

                if (!shim_loaded) {
                    LOGD("Loading shim");
                    JniHelper::shim_loaded = PrepareAssetJar("XliShimJ.apk","XliJ");
                    LOGD("Done loading");
                }

                jclass *shim_class = new jclass;
                *shim_class = GetAssetClass("XliShimJ.apk","XliJ");
                
                pthread_setspecific(JniThreadKey, (void*)env);
                pthread_setspecific(JniShimKey, (void*)shim_class);
                
                AttachNativeCallbacks(shim_class, env);
                AttachHiddenView(shim_class, env, AndroidActivity->clazz);
            }
		}
        jclass* shim_p = reinterpret_cast<jclass*>(pthread_getspecific(JniShimKey));
        if (shim_p)
        {
            shim = *shim_p;
        } else {
            LOGE("could not get shim");
        }
	}

    jclass JniHelper::GetShim()
    {
        if (!shim_loaded) 
        { 
            LOGE("Shim isnt loaded yet");
            return 0;
        } else {
              return shim;
        }
    }

	jmethodID JniHelper::FindMethod(const char* className, const char* methodName, const char* methodSig)
	{
		jclass cls = env->FindClass(className);
		if (!cls) XLI_THROW((String)"Failed to get JNI class '" + className + "'");

		jmethodID method = env->GetMethodID(cls, methodName, methodSig);
		if (!method) XLI_THROW((String)"Failed to get JNI method '" + className + "." + methodName + methodSig + "'");

		return method;
	}

	jobject JniHelper::CallObjectMethod(jobject inst, const char* name, const char* sig)
	{
		jclass cls = env->GetObjectClass(inst);

#ifdef DEBUG_JNI
		if (!cls) XLI_THROW((String)"Failed to get JNI class for method '" + name + "'");
#endif

		jmethodID method = env->GetMethodID(cls, name, sig);

#ifdef DEBUG_JNI
		if (!method) XLI_THROW((String)"Failed to get JNI method '" + name + "'");
#endif

		return env->CallObjectMethod(inst, method);
	}

	String JniHelper::GetString(jobject str)
	{
		const char* utf8 = env->GetStringUTFChars((jstring)str, NULL);
		String result = utf8;
		env->ReleaseStringUTFChars((jstring)str, utf8);
		return result;
	}

	JNIEnv* JniHelper::GetEnv()
	{
		// TODO: Check jni exceptions
		return env;
	}

	JNIEnv* JniHelper::operator->()
	{
		// TODO: Check jni exceptions
		return env;
	}
    
    jmethodID JniHelper::GetInstanceMethod(const char* m_name, const char* m_sig)
    {
        return GetInstanceMethod(GetInstance(), m_name, m_sig);
    }

    jmethodID JniHelper::GetInstanceMethod(jobject inst, const char* m_name, const char* m_sig)
    {
        return env->GetMethodID(env->GetObjectClass(inst), m_name, m_sig);
    }

    jobject JniHelper::GetInstance() 
    {
        return AndroidActivity->clazz;
    }

    jobject JniHelper::GetInstance(const char* class_name, const char* constructor_sig, ...)
    {
        jclass cls = env->FindClass(class_name);
        if (cls) {
            jmethodID mid = env->GetMethodID(cls, "<init>", constructor_sig);
            if (mid) {
                jobject result = 0;
                va_list args;
                va_start(args, constructor_sig);
                result = env->NewObjectV(cls, mid, args);
                va_end(args);
                return result;
            } else {
                LOGE( "GetInstance Could not get <init> for class");
                return NULL;
            }
        } else {
            LOGE( "GetInstance Could not find class");
            return NULL;
        }
    }

    jobject JniHelper::GetInstance(jclass cls, const char* constructor_sig, ...)
    {
        jmethodID mid = env->GetMethodID(cls, "<init>", constructor_sig);
        jobject result = 0;
        va_list args;
        va_start(args, constructor_sig);
        result = env->NewObjectV(cls, mid, args);
        va_end(args);
        return result;
    }

    jclass JniHelper::GetCustomClass(const char* class_path)
    {
        jclass activityClass = env->FindClass("android/app/NativeActivity");
        jmethodID getClassLoader = env->GetMethodID(activityClass,"getClassLoader", "()Ljava/lang/ClassLoader;");
        jobject cls = env->CallObjectMethod(AndroidActivity->clazz, getClassLoader);
        jclass classLoader = env->FindClass("java/lang/ClassLoader");
        jmethodID findClass = env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        jstring strClassName = env->NewStringUTF(class_path);
        jclass classIWant = (jclass)env->CallObjectMethod(cls, findClass, strClassName);
    }

    int JniHelper::PrepareAssetJar(const char* file_name, const char* class_name) 
    {
        return PrepareAssetJar(file_name, class_name, 1);
    }

    int JniHelper::PrepareAssetJar(const char* file_name, const char* class_name, int package)
    {
        jmethodID get_dir = GetInstanceMethod("getDir","(Ljava/lang/String;I)Ljava/io/File;");
        if (!get_dir) { LOGE("Could not find get_dir"); return 0; }

        //dex-file
        jobject dex_dir_file = env->CallObjectMethod(AndroidActivity->clazz, get_dir, env->NewStringUTF("dex"), (jint)0);
        if (!dex_dir_file) { LOGE("Could not find dex_dir_file"); return 0; }

        //Find destination file
        jobject dex_file = GetInstance("java/io/File","(Ljava/io/File;Ljava/lang/String;)V", dex_dir_file, env->NewStringUTF(file_name));
        if (!dex_file) { LOGE("Could not find dex_file"); return 0; }
        
        //get destination path
        jmethodID getAbsPath = env->GetMethodID(env->FindClass("java/io/File"), "getAbsolutePath", "()Ljava/lang/String;");
        jstring dex_internal_path = (jstring)env->CallObjectMethod(dex_file, getAbsPath);
        const char *filepath = env->GetStringUTFChars(dex_internal_path, 0);
        
        //write apk
        FILE* appConfigFile = fopen(filepath, "w+");
        int file_len = sizeof(Xli::shimApkData);
        LOGI("App config file created successfully. Writing config data ...\n");
        int32_t res = fwrite((void*)Xli::shimApkData, sizeof(char), file_len, appConfigFile);
        if (file_len != res)
        {
            LOGE("Error generating apk.\n");
            return 0;
        }
        fclose(appConfigFile);
        env->ReleaseStringUTFChars(dex_internal_path, filepath);
        return 1;
    }


    jclass JniHelper::GetAssetClass(const char* file_name, const char* class_name)
    {
        jmethodID get_dir = GetInstanceMethod("getDir","(Ljava/lang/String;I)Ljava/io/File;");

        jobject dex_dir_file = env->CallObjectMethod(AndroidActivity->clazz, get_dir, env->NewStringUTF("dex"), (jint)0);
        jclass file_cls = env->FindClass("java/io/File");
        jmethodID getAbsPath = env->GetMethodID(file_cls, "getAbsolutePath", "()Ljava/lang/String;");

        jclass dexloader_cls = env->FindClass("dalvik/system/DexClassLoader");
        jmethodID load_class = env->GetMethodID(dexloader_cls, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        jmethodID get_classloader = GetInstanceMethod("getClassLoader","()Ljava/lang/ClassLoader;");
        jobject classloader = env->CallObjectMethod(AndroidActivity->clazz, get_classloader);

        jobject dex_file = GetInstance("java/io/File","(Ljava/io/File;Ljava/lang/String;)V", dex_dir_file, env->NewStringUTF(file_name));
        jobject outdex_dir_file = env->CallObjectMethod(AndroidActivity->clazz, get_dir, env->NewStringUTF("outdex"), (jint)0);
        jstring dex_optimized_out_path = (jstring)env->CallObjectMethod(outdex_dir_file, getAbsPath);
        jstring dex_internal_path = (jstring)env->CallObjectMethod(dex_file, getAbsPath);

        jobject dexloader = GetInstance("dalvik/system/DexClassLoader", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V", dex_internal_path, dex_optimized_out_path, NULL, classloader);

        jclass loaded = (jclass)env->CallObjectMethod(dexloader, load_class, env->NewStringUTF(class_name));
        jclass globalClass = reinterpret_cast<jclass>(env->NewGlobalRef(loaded));
        return globalClass;
    }
}
