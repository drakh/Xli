#include "AInternal.h"
#include <pthread.h>

namespace Xli
{
	struct ANativeActivity* AndroidActivity = 0;
	
	static pthread_key_t JniThreadKey;

	static void JniDestroyThread(void* value)
	{
		LOGD("JNI: Detaching current thread");

		JNIEnv* env = (JNIEnv*)value;
		AndroidActivity->vm->DetachCurrentThread();
		pthread_setspecific(JniThreadKey, NULL);
	}

	void JniHelper::Init()
	{
		if (pthread_key_create(&JniThreadKey, JniDestroyThread))
			LOGE("JNI ERROR: Unable to create pthread key"); // Not fatal
	}

	JniHelper::JniHelper()
	{
		if (AndroidActivity->vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
		{
			if (AndroidActivity->vm->AttachCurrentThread(&env, NULL) != JNI_OK)
				XLI_THROW("JNI ERROR: Failed to attach current thread");

			pthread_setspecific(JniThreadKey, (void*)env);
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
}
