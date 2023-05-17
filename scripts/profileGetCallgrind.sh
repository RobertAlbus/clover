#!/bin/bash

rm -fr ./build/CloverDemo
rm ./callgrind.out
cmake -B build -G "Unix Makefiles" -DPROFILING_MODE=ON;
make -C build;
valgrind --tool=callgrind --callgrind-out-file=./callgrind.out ./build/CloverDemo

# Visualize the Callgrind output using KCachegrind (or another compatible tool)
kcachegrind ./callgrind.out &