#!/bin/sh
set -e
cd "$( dirname "${BASH_SOURCE[0]}" )"

mkdir -p builds/xcode
cd builds/xcode

cmake ../../ -GXcode
xcodebuild -alltargets -configuration Debug
xcodebuild -alltargets -configuration Release
