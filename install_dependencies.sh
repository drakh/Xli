#!/bin/sh
set -e
cd "`dirname "$0"`"

if which apt-get; then

	apt-get install libpng12-dev libjpeg-dev libfreetype6-dev libglew-dev portaudio19-dev libcurl4-openssl-dev cmake mercurial g++

elif which port; then

	port install cmake mercurial portaudio +universal

else
	echo "FATAL ERROR: Unsupported system" >&2
	exit 1
fi


# Download and build SDL2

if [ -d src/core/3rdparty/SDL2/.hg ]; then
    cd src/core/3rdparty/SDL2
    hg pull && hg update
else
	cd src/core/3rdparty
	rm -rf SDL2
	hg clone http://hg.libsdl.org/SDL SDL2
	cd SDL2
fi

if [ `uname` = "Darwin" ]; then

	xcodebuild -project 'Xcode/SDL/SDL.xcodeproj' -configuration 'Release' -target 'Static Library' CONFIGURATION_BUILD_DIR='../../lib'

else

	mkdir -p build
	cd build

	../configure --srcdir=.. --prefix=/usr
	make -j `grep processor /proc/cpuinfo | wc -l`
	make install

fi
