#!/bin/bash

# Build project
mkdir build
cd build || exit
cmake ..
cmake --build . --config Debug --target all -- -j 10
