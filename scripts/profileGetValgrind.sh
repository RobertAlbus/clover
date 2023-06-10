#!/bin/bash

rm -fr ./build/CloverDemo;
cmake -B build -G "Unix Makefiles" -DPROFILING_MODE=true;
make -C build;
LD_PRELOAD=/usr/local/lib/prof.so CPUPROFILE_FREQUENCY=48000 CPUPROFILE=./test.prof ./build/CloverDemo && \
pprof -web -cum ./build/CloverDemo ./test.prof