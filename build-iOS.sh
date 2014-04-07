#!/bin/sh
set -e

if [ "$CMAKE" = "1" ]; then

	mkdir -p build/iOS
	cd build/iOS

	rm -f CMakeCache.txt
	cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain/iOS.cmake -GXcode ../..

	xcodebuild -alltargets -configuration Debug
	xcodebuild -alltargets -configuration Release

else

	cd "`dirname "$0"`/projects/iOS"
	xcodebuild -alltargets -configuration Debug -sdk iphoneos
	xcodebuild -alltargets -configuration Release -sdk iphoneos
	xcodebuild -alltargets -configuration Debug -sdk iphonesimulator
	xcodebuild -alltargets -configuration Release -sdk iphonesimulator

fi
