#!/bin/sh

cd projects/ios || exit $?

xcodebuild -alltargets -configuration Debug -sdk iphoneos
xcodebuild -alltargets -configuration Release -sdk iphoneos
xcodebuild -alltargets -configuration Debug -sdk iphonesimulator
xcodebuild -alltargets -configuration Release -sdk iphonesimulator

cd -
