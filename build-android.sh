#!/bin/sh	
set -e
cd "`dirname "$0"`"

rm -rf lib/android

if [ "$@ " = "--clean " ]; then
    rm -rf projects/android/libs
    rm -rf projects/android/obj
    exit 0
fi

if [ "$@ " = "--shim " ]; then
    cd src/core/android/shim
    android update project --target android-19 --name XliShim --path .
    ant debug
    hexdump -v -e '"" 20/1 "0x%02x," "\n"' "./bin/XliShim-debug.apk" | sed -e '$s/0x  ,//g' >  "apk.inc"
    exit 0
fi

if [ -z "$JOB_COUNT" ]; then
    if [ -f /proc/cpuinfo ]; then
        JOB_COUNT=`grep processor /proc/cpuinfo | wc -l`
    elif [ `uname` = "Darwin" ]; then
        JOB_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
    elif [ -n "$NUMBER_OF_PROCESSORS" ]; then
        JOB_COUNT=$NUMBER_OF_PROCESSORS
    else
        JOB_COUNT=1 
    fi
fi

cd projects/android
rm -rf libs
ndk-build -j $JOB_COUNT $@
cd -

SOURCE="projects/android/obj/local"
TARGET="lib/android"

if [ -n "`which rsync`" ]; then
	CP_CMD="rsync -vru"
else
	CP_CMD="cp -vru"
fi

for arch in armeabi armeabi-v7a x86; do
	if [ -d "$SOURCE/$arch" ]; then
        mkdir -p "$TARGET/$arch"
		$CP_CMD $SOURCE/$arch/*.so "$TARGET/$arch"
        $CP_CMD $SOURCE/$arch/*.a "$TARGET/$arch"
	fi
done
