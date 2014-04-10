#!/bin/sh
set -e
cd "`dirname "$0"`"
PATH="utils:$PATH"
cmakemake "$@" .
