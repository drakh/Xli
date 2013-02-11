#!/bin/sh

cd projects/iOS || exit $?

xcodebuild -alltargets -configuration Debug -sdk iphoneos
xcodebuild -alltargets -configuration Release -sdk iphoneos
xcodebuild -alltargets -configuration Debug -sdk iphonesimulator
xcodebuild -alltargets -configuration Release -sdk iphonesimulator

cd -
