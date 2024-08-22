#!/bin/bash
if [[ ! -d "./build" ]]; then
    mkdir -p build
fi
rm -r ./build/*
cmake -B ./build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build ./build --config Release
