#!/bin/sh
set -e
cd "$( dirname "${BASH_SOURCE[0]}" )"

if [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
fi

if [ -d src/core/3rdparty/SDL2 ]; then
    cd src/core/3rdparty/SDL2
    hg pull
    hg update
    cd -
else
    cd src/core/3rdparty
    hg clone http://hg.libsdl.org/SDL SDL2
    cd -
fi

mkdir -p src/core/3rdparty/SDL2/build
cd src/core/3rdparty/SDL2/build

../configure --srcdir=.. --prefix=/usr --disable-video-opengles
make -j $CPU_COUNT
sudo make install
