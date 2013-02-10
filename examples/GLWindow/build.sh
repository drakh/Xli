#!/bin/sh

mkdir -p builds/cmake
cd builds/cmake

cmake ../.. && make

cd -
