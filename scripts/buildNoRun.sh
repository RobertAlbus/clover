#!/bin/bash

cmake -B build -G "Unix Makefiles";
touch build/.gitkeep
make -C build;