#!/bin/sh    
set -e
cd "`dirname "$0"`"

DEBUG=0

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
    DEBUG=1
    ;;
esac

XLI_DIR="`pwd -P`"
NDK_FILENAME=`which ndk-build`
NDK_DIR=`dirname "$NDK_FILENAME"`
NDK_ABI="armeabi-v7a"

if [ "$DEBUG" = "1" ]; then
    BUILD_TYPE="Debug"
else
    BUILD_TYPE="Release"
fi

BUILD_DIR="build/android/$BUILD_TYPE-$NDK_ABI"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

set -- "$@" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
set -- "$@" -DCMAKE_TOOLCHAIN_FILE="$XLI_DIR/cmake/toolchain/android.toolchain.cmake" 
set -- "$@" -DANDROID_NDK="$NDK_DIR" 
set -- "$@" -DANDROID_ABI="$NDK_ABI" 
set -- "$@" -DANDROID_NATIVE_API_LEVEL="android-9"
set -- "$@" -DANDROID_STL="stlport_static"

cmake "$@" "$XLI_DIR"

if [ -f /proc/cpuinfo ]; then
    JOB_COUNT=`grep processor /proc/cpuinfo | wc -l`
elif [ "`uname`" = "Darwin" ]; then
    JOB_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
elif [ -n "$NUMBER_OF_PROCESSORS" ]; then
    JOB_COUNT=$NUMBER_OF_PROCESSORS
else
    JOB_COUNT=1
fi

make -j $JOB_COUNT
