#!/bin/bash

# Default is debug build
CONFIGURATION=Debug
# Install game?
INSTALL=0

# Check for arguments
while test $# -gt 0; do
  case "$1" in
  -d | --debug | --Debug)
    CONFIGURATION=Debug
    ;;
  -r | --release | --Release)
    CONFIGURATION=Release
    ;;
  -c | --clean)
    rm -rf build
    ;;
  -i | --install)
    INSTALL=1
    ;;
  -*)
    echo -e "\033[0;31mBad argument: $1\033[0m"
    exit 1
    ;;
  *)
    echo -e "\033[0;31mUnknown argument: $1\033[0m"
    exit 1
    ;;
  esac
  shift
done

# Build project
echo -e "\033[0;34mBuilding a ${CONFIGURATION} version...\033[0m"

# Create build dir if not exists
if [ ! -d "build" ]; then
  mkdir build
fi

# Fail if couldn't cd into dir
cd build || exit 1

# Create version dir
if [ ! -d ${CONFIGURATION} ]; then
  mkdir ${CONFIGURATION}
fi

# Fail if couldn't cd into dir
cd ${CONFIGURATION} || exit 1

# Run CMake
cmake ../.. -DCMAKE_BUILD_TYPE=${CONFIGURATION} -DCMAKE_INSTALL_PREFIX=./bin
if [ $INSTALL = 1 ]; then
  make
  make install
else
  cmake --build . --target all -- -j 10
fi

# Exit script successfully
exit 0
