#!/bin/sh
cmake -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release . && \
make -j `grep processor /proc/cpuinfo | wc -l`
