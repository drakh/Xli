#!/bin/sh
mkdir -p jni && \
cp -vru *.mk src jni && \
ndk-build -j `grep processor /proc/cpuinfo | wc -l` && \
cp -v obj/local/armeabi-v7a/libXliAndroidStatic.a lib/libXliAndroid.a
