#!/bin/sh
set -e
cd "$( dirname "${BASH_SOURCE[0]}" )"

if [ -f /proc/cpuinfo ]; then
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`

	if [ `uname -o` = "Cygwin" ]; then
	    chmod -R 0777 src
	fi

elif [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=1	
fi

cd projects/android

ndk-build -j $CPU_COUNT

mkdir -p ../../lib/android

if [ -d obj/local/armeabi ]; then
	mkdir -p ../../lib/android/armeabi && \
	cp -v obj/local/armeabi/libXli*.a ../../lib/android/armeabi
fi

if [ -d obj/local/armeabi-v7a ]; then
	mkdir -p ../../lib/android/armeabi-v7a && \
	cp -v obj/local/armeabi-v7a/libXli*.a ../../lib/android/armeabi-v7a
fi

if [ -d obj/local/x86 ]; then
	mkdir -p ../../lib/android/x86 && \
	cp -v obj/local/x86/libXli*.a ../../lib/android/x86
fi
