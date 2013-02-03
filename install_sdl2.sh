#!/bin/sh
if [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
fi

if [ -d SDL2 ]; then
    cd SDL2
    hg update
    cd -
else
    hg clone http://hg.libsdl.org/SDL SDL2
fi

mkdir -p SDL2/build
cd SDL2/build || exit $?

../configure --srcdir=.. --prefix=/usr && \
make -j $CPU_COUNT && \
sudo make install

cd -

