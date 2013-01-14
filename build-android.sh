#!/bin/sh
if [ `uname -o` = "Cygwin" ]; then
	chmod -R 0777 src
fi

if [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
fi

cd projects/android-ndk
ndk-build -j $CPU_COUNT && \

if [ -f obj/local/armeabi/libXliAndroidStatic.a ]; then
	mkdir -p ../../lib/armeabi && \
	cp -v obj/local/armeabi/libXliAndroidStatic.a ../../lib/armeabi
fi

if [ -f obj/local/armeabi-v7a/libXliAndroidStatic.a ]; then
	mkdir -p ../../lib/armeabi-v7a && \
	cp -v obj/local/armeabi-v7a/libXliAndroidStatic.a ../../lib/armeabi-v7a
fi

if [ -f obj/local/x86/libXliAndroidStatic.a ]; then
	mkdir -p ../../lib/x86 && \
	cp -v obj/local/x86/libXliAndroidStatic.a ../../lib/x86
fi

cd -
