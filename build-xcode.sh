#!/bin/sh

mkdir -p builds/xcode
cd builds/xcode

cmake ../../ -GXcode
xcodebuild -alltargets -configuration Debug
xcodebuild -alltargets -configuration Release

cd -
