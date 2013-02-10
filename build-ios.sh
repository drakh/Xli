#!/bin/sh
xcodebuild -project projects/ios/Xli.xcodeproj -alltargets -configuration Debug -sdk iphoneos
xcodebuild -project projects/ios/Xli.xcodeproj -alltargets -configuration Release -sdk iphoneos
xcodebuild -project projects/ios/Xli.xcodeproj -alltargets -configuration Debug -sdk iphonesimulator
xcodebuild -project projects/ios/Xli.xcodeproj -alltargets -configuration Release -sdk iphonesimulator
