#!/bin/sh
set -e
cd "`dirname "$0"`"

XLI_DIR="`pwd -P`"

set -- -DCMAKE_TOOLCHAIN_FILE="$XLI_DIR/cmake/toolchain/iOS.cmake" -GXcode "$@"

mkdir -p build/iphoneos
cd build/iphoneos

cmake "$@" -DIOS_PLATFORM=OS "$XLI_DIR"

xcodebuild -alltargets -configuration Debug -sdk iphoneos
xcodebuild -alltargets -configuration Release -sdk iphoneos

cd "$XLI_DIR"
mkdir -p build/iphonesimulator
cd build/iphonesimulator

cmake "$@" -DIOS_PLATFORM=SIMULATOR "$XLI_DIR"

xcodebuild -alltargets -configuration Debug -sdk iphonesimulator
xcodebuild -alltargets -configuration Release -sdk iphonesimulator
