#!/bin/sh
set -e

if which apt-get; then

	apt-get install libpng12-dev libjpeg-dev libfreetype6-dev libglew-dev portaudio19-dev libcurl4-openssl-dev cmake mercurial g++

elif which port; then

	port install cmake mercurial portaudio +universal

else

	echo "FATAL ERROR: Unsupported system, patches welcome :)"
	exit 1

fi

if [ -d src/core/3rdparty/SDL2/.hg ]; then
    cd src/core/3rdparty/SDL2
    hg pull && hg update
else
	cd src/core/3rdparty
	rm -rf SDL2
	hg clone http://hg.libsdl.org/SDL SDL2
	cd SDL2
fi

mkdir -p build
cd build

if [ -f /proc/cpuinfo ]; then
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
elif [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=1
fi

../configure --srcdir=.. --prefix=/usr
make -j $CPU_COUNT
make install

