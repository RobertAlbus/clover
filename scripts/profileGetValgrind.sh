#!/bin/bash

rm -fr ./build/CloverDemo;
cmake -B build -G "Unix Makefiles" -DPROFILING_MODE=true;
make -C build;
LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=./test.prof ./build/CloverDemo && \
pprof -web -cum ./build/CloverDemo ./test.prof