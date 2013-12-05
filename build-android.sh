#!/bin/sh	
set -e
cd "`dirname "$0"`"

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
ndk-build -j $JOB_COUNT $@ || true
cd -

SOURCE="projects/android/obj/local"
TARGET="lib/android"

mkdir -p "$TARGET"

if [ -n "`which rsync`" ]; then
	CP_CMD="rsync -vru"
else
	CP_CMD="cp -vru"
fi

for arch in armeabi armeabi-v7a x86; do
	if [ -d "$SOURCE/$arch" ]; then
		mkdir -p "$TARGET/$arch"
		$CP_CMD $SOURCE/$arch/libXli*.a "$TARGET/$arch"
	fi
done
