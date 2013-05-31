@echo off
mkdir build\mingw
cd build\mingw
cmake ..\.. -G"Unix Makefiles"
make
cd ..\..
