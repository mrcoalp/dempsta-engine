#!/bin/bash

# Default is debug build
BUILD=Debug

while test $# -gt 0; do
  case "$1" in
  -d | --debug)
    BUILD=Debug
    ;;
  -r | --release)
    BUILD=Release
    ;;
  -c | --clean)
    rm -rf build
    ;;
  -*)
    echo "Bad argument: $1"
    exit 1
    ;;
  *)
    echo "Unknown argument: $1"
    exit 1
    ;;
  esac
  shift
done

# Build project
echo "Building a ${BUILD} version!"
mkdir build
cd build || exit 1
cmake .. -DCMAKE_BUILD_TYPE=${BUILD}
cmake --build . --target all -- -j 10

exit 0
