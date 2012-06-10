#!/bin/sh
hg clone http://hg.libsdl.org/SDL SDL2
mkdir SDL2/build
cd SDL2/build
../configure --srcdir=.. --prefix=/usr
make -j 8
sudo make install
cd - 
