#!/bin/bash

cmake -B build;
make -C build && sh scripts/run.sh