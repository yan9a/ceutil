#!/bin/bash
rm -r build/
cmake -B ./build -S .
cmake --build ./build
cd build

