#!/bin/sh
set -e

cd "`dirname "$0"`"
XLI_DIR="`pwd -P`"

cd "$XLI_DIR"
mkdir -p build/iphoneos
cd build/iphoneos

cmake -DCMAKE_TOOLCHAIN_FILE="$XLI_DIR/cmake/toolchain/iOS.cmake" -DIOS_PLATFORM=OS -GXcode "$XLI_DIR"

xcodebuild -alltargets -configuration Debug -sdk iphoneos
xcodebuild -alltargets -configuration Release -sdk iphoneos

cd "$XLI_DIR"
mkdir -p build/iphonesimulator
cd build/iphonesimulator

cmake -DCMAKE_TOOLCHAIN_FILE="$XLI_DIR/cmake/toolchain/iOS.cmake" -DIOS_PLATFORM=SIMULATOR -GXcode "$XLI_DIR"

xcodebuild -alltargets -configuration Debug -sdk iphonesimulator
xcodebuild -alltargets -configuration Release -sdk iphonesimulator
