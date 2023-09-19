#!/bin/bash

git submodule update --init --recursive

cmake -B build -G "Unix Makefiles";
touch build/.gitkeep