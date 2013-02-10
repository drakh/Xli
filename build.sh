#!/bin/sh
if [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
fi

mkdir -p builds/cmake
cd builds/cmake || exit $?

rm -f CMakeCache.txt
cmake -G"Unix Makefiles" ../.. && \
make -j $CPU_COUNT

cd -
