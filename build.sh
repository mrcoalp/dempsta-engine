#!/bin/bash

# Default is debug build
BUILD=Debug

# Check for arguments
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

# Create build dir if not exists
if [ ! -d "build" ]; then
  mkdir build
fi

# Fail if couldn't cd into dir
cd build || exit 1

# Create version dir
if [ ! -d ${BUILD} ]; then
  mkdir ${BUILD}
fi

# Fail if couldn't cd into dir
cd ${BUILD} || exit 1

# Run CMake
cmake ../.. -DCMAKE_BUILD_TYPE=${BUILD}
cmake --build . --target all -- -j 10

# Exit script successfully
exit 0
