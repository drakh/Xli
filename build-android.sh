#!/bin/sh    
set -e
cd "`dirname "$0"`"

case $1 in
shim)
    cd src/XliPlatform/android/shim
    android update project --target android-19 --name XliShim --path .
    ant debug
    hexdump -ve '"" 20/1 "0x%02x," "\n"' "./bin/XliShim-debug.apk" | sed -e '$s/0x  ,//g' > "apk.inc"
    exit 0
    ;;
esac

scripts/cmakemake "$@" --platform=android .
