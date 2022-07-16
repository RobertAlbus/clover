#!/bin/bash

g++ -lasound -lm -lpthread -pthread -lportaudio -o bin/out paex_sine_c++.cpp /usr/lib/libportaudio.so && bin/out

