#!/bin/bash
mkdir build
mkdir result
cd build
cmake ..
make clean
make
