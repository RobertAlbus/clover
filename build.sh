#!/bin/bash

g++ -lasound -lm -lpthread -pthread -lportaudio -o bin/main main.cpp /usr/lib/libportaudio.so && bin/main

