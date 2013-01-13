#!/bin/sh
sudo port install mercurial

if [ -d src/platform/sdl2/SDL2-iOS ]; then
    cd src/platform/sdl2/SDL2-iOS
    hg update
    cd -
else
    cd src/platform/sdl2
    hg clone http://hg.libsdl.org/SDL SDL2-iOS
    cd -
fi
