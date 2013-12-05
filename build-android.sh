#!/bin/sh	
set -e
cd "`dirname "$0"`"

if [ -f /proc/cpuinfo ]; then
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
elif [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=1	
fi

cd projects/android
ndk-build -j $CPU_COUNT $@ || true
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
