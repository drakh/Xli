#!/bin/sh
if [ `uname` = "Darwin" ]; then
	CpuCount=`sysctl hw.ncpu | cut -d " " -f 2`
else
	CpuCount=`grep processor /proc/cpuinfo | wc -l`
fi

cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release . && \
make -j $CpuCount
