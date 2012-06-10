#!/bin/sh
xcodebuild -project projects/Xcode-iOS/Xli-iOS.xcodeproj -configuration Debug -sdk iphoneos
xcodebuild -project projects/Xcode-iOS/Xli-iOS.xcodeproj -configuration Release -sdk iphoneos
xcodebuild -project projects/Xcode-iOS/Xli-iOS.xcodeproj -configuration Debug -sdk iphonesimulator
xcodebuild -project projects/Xcode-iOS/Xli-iOS.xcodeproj -configuration Release -sdk iphonesimulator
