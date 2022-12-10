#!/bin/bash

rm -fr ./build/CloverDemo;
sh scripts/buildNoRun.sh;
LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=./test.prof ./build/CloverDemo && \
sh scripts/profileView.sh;