#!/bin/sh
if [ `uname` = "Darwin" ]; then
	CPU_COUNT=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CPU_COUNT=`grep processor /proc/cpuinfo | wc -l`
fi

mkdir -p cmake-build
cd cmake-build || exit $?

cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. && \
make -j $CPU_COUNT

cd -

