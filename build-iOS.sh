#!/bin/sh
set -e
cd "$( dirname "${BASH_SOURCE[0]}" )"

cd projects/iOS

xcodebuild -alltargets -configuration Debug -sdk iphoneos
xcodebuild -alltargets -configuration Release -sdk iphoneos
xcodebuild -alltargets -configuration Debug -sdk iphonesimulator
xcodebuild -alltargets -configuration Release -sdk iphonesimulator
