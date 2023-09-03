#!/bin/bash

cmake -B build -G "Unix Makefiles" -DENABLE_WARNINGS=ON;
touch build/.gitkeep
make -C build -j8;
