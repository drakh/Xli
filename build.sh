#!/bin/sh
set -e
cd "`dirname "$0"`"
PATH="$PATH:utils"

cmakemake "$@" .
