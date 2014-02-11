#!/bin/sh	
set -e
cd "`dirname "$0"`"

case $1 in
clean)
    rm -rfv projects/android/libs
    rm -rfv projects/android/obj
    rm -rfv lib/android
    exit 0
    ;;
shim)
    cd src/core/android/shim
    android update project --target android-19 --name XliShim --path .
    ant debug
    hexdump -ve '"" 20/1 "0x%02x," "\n"' "./bin/XliShim-debug.apk" | sed -e '$s/0x  ,//g' > "apk.inc"
    exit 0
    ;;
--debug)
    shift
    set APP_OPTIM=debug "$@"
    ;;
esac

if [ -f /proc/cpuinfo ]; then
    JOB_COUNT=`grep processor /proc/cpuinfo | wc -l`
elif [ "$OSTYPE" = darwin* ]; then
    JOB_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
elif [ -n "$NUMBER_OF_PROCESSORS" ]; then
    JOB_COUNT=$NUMBER_OF_PROCESSORS
else
    JOB_COUNT=1
fi

SOURCE="projects/android"
TARGET="lib/android"

cd "$SOURCE"
ndk-build -j $JOB_COUNT "$@"
cd "$OLDPWD"

if which rsync > /dev/null 2>&1; then
	CP_CMD="rsync -vru"
else
	CP_CMD="cp -vru"
fi

for arch in "armeabi" "armeabi-v7a" "x86"; do
	if [ -d "$SOURCE/obj/local/$arch" ]; then
        mkdir -p "$TARGET/$arch"
		$CP_CMD "$SOURCE/obj/local/$arch/"*.a "$SOURCE/libs/$arch/"*.so "$TARGET/$arch"
	fi
done
