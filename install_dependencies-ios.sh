#!/bin/sh
sudo port install mercurial

if [ -d src/core/sdl2/SDL2-iOS ]; then
    cd src/core/sdl2/SDL2-iOS
    hg update
    cd -
else
    cd src/core/sdl2
    hg clone http://hg.libsdl.org/SDL SDL2-iOS
    cd -
fi
