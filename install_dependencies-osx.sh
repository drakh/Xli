#!/bin/sh
sudo port install cmake
sudo port install mercurial
sudo port install portaudio +universal

if [ -d src/core/3rdparty/SDL2 ]; then
    cd src/core/3rdparty/SDL2
    hg update
    cd -
else
    cd src/core/3rdparty
    hg clone http://hg.libsdl.org/SDL SDL2
    cd -
fi

xcodebuild -project 'src/core/3rdparty/SDL2/Xcode/SDL/SDL.xcodeproj' -configuration 'Release' -target 'Static Library' CONFIGURATION_BUILD_DIR='../../lib'
