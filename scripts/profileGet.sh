#!/bin/bash

LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=test.prof ./build/Clover && \
sh scripts/profileView.sh;