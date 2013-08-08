#!/bin/sh
START_DIR=`dirname "$0"`
PATH="$PATH:$START_DIR/../utils"

set -e
cmakemake $@ "$START_DIR"

