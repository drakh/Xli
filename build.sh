#!/bin/sh
if [ -f /proc/cpuinfo ]; then
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
elif [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=1
fi

mkdir -p builds/cmake
cd builds/cmake || exit $?

rm -f CMakeCache.txt
cmake ../.. && make -j $CPU_COUNT

cd -
