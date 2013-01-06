#!/bin/sh
mkdir -p jni && \
cp -vru *.mk src jni || exit $?
 
if [ `uname -o` = "Cygwin" ]; then
	chmod -R 0777 jni
fi

ndk-build -j `grep processor /proc/cpuinfo | wc -l` && \
cp -v obj/local/armeabi-v7a/libXliAndroidStatic.a lib/libXliAndroid.a
