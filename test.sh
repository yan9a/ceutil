#!/bin/bash
cd test
if [[ ! -d "./build" ]]; then
    mkdir -p build
fi
rm -r ./build/*
cmake -B ./build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build ./build --config Release
cd build
ctest -N
ctest --verbose -C Release
cd ..
