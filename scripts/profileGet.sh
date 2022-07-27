#!/bin/bash

LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=test.prof ./build/TryPort && \
sh scripts/profileView.sh;