#!/bin/bash
hexdump -v -e '"" 20/1 "0x%02x," "\n"' "./bin/XliShimJ.apk" | sed -e '$s/0x  ,//g' >  "../shimApkData.h"
