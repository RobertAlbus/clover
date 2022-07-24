#!/bin/bash

rm -fr ./build/*;
cmake -B build;
make -C build;
./build/TryPort