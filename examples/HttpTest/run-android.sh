#!/bin/sh

CLASS="HttpTest"
PACKAGE="com.xli.httptest"

set -e
cd "`dirname "$0"`/project-android"

# Build Xli
../../../build-android.sh --debug

# Build .so
ndk-build

# Build .apk
android update project --target "android-19" --name $CLASS --path .
ant debug

# Install and run
adb uninstall $PACKAGE
adb install -r bin/$CLASS-debug.apk
adb shell setprop debug.checkjni 1
adb shell "logcat -c && am start -a android.intent.action.MAIN -c [android.intent.category.LAUNCHER] -f 0x10200000 -n $PACKAGE/$PACKAGE.$CLASS && logcat -s XliApp"