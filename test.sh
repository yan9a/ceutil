#!/bin/bash
cd test
cmake -B ./build -S .
cmake --build ./build
cd build
# ctest -N # list
ctest --verbose 
