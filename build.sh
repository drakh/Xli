#!/bin/sh
START_DIR="$( dirname "${BASH_SOURCE[0]}" )"
PATH=$PATH:$START_DIR/utils
cmakemake $@ $START_DIR || exit 1
