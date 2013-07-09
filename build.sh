#!/bin/sh
if [ -f /proc/cpuinfo ]; then
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
elif [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=1
fi

if [ $DEBUG ]; then
	BUILD_DIR="builds/cmake-debug"
else
	BUILD_DIR="builds/cmake"
fi

mkdir -p $BUILD_DIR
cd $BUILD_DIR || exit $?

rm -f CMakeCache.txt
cmake ../.. && make -j $CPU_COUNT

cd -
