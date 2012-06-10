#!/bin/sh
if [ -d src/platform/sdl2/SDL2-iOS ]; then
    cd src/platform/sdl2/SDL2-iOS
    hg update
    cd -
else
    cd src/platform/sdl2
    hg clone http://hg.libsdl.org/SDL SDL2-iOS
    cd -
    cd src/platform/sdl2/SDL2-iOS
    hg patch ../SDL2-iOS-patch.diff --force --no-commit
    cd -
fi
