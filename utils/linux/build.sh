#!/bin/bash

# Default is debug build
CONFIGURATION=Debug
# Install game?
INSTALL=0
# Use Ninja?
BUILD_SYSTEM=-GNinja
# Only generate compilation db
CHECK_ONLY=0
# Wether or not to build tests
TESTS=OFF
# Whether or not to generate documentation
DOC=OFF

usage() {
  echo "Usage

    build.sh [options]

    Builds the game, by default, with debug configuration.

Options
    -h | --help       - Show help
    -d | --debug      - Build game with debug configuration
    -r | --release    - Build game with release configuration
    -c | --clean      - Clean build of configuration
    -i | --install    - Install game to build/bin folder
    --ninja           - Use Ninja as build system (default)
    --make            - Use Make as build system
    --cc <#version>   - Specify which GCC compiler <#version> to use. Ex: --cc 9
    --clang           - Use Clang compiler
    --check           - Only generate compilation database
    -t | --tests      - Build tests
    --doc             - Generate docs"
}

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
    rm -rf build/$CONFIGURATION
    ;;
  -i | --install)
    INSTALL=1
    ;;
  --ninja)
    BUILD_SYSTEM=-GNinja
    ;;
  --make)
    BUILD_SYSTEM=
    ;;
  --cc)
    export CC=/usr/bin/gcc-$2
    export CXX=/usr/bin/g++-$2
    shift
    ;;
  --clang)
    export CC=/usr/bin/clang
    export CXX=/usr/bin/clang++
    ;;
  --check)
    CHECK_ONLY=1
    ;;
  -t | --tests)
    TESTS=ON
    ;;
  --doc)
    DOC=ON
    ;;
  -h | --help)
    usage
    exit 0
    ;;
  -*)
    echo -e "\033[0;31mBad argument: $1\033[0m"
    ./build.sh -h
    exit 1
    ;;
  *)
    echo -e "\033[0;31mUnknown argument: $1\033[0m"
    ./build.sh -h
    exit 1
    ;;
  esac
  shift
done

# Build project
echo -e "\033[0;34mBuilding a $CONFIGURATION version...\033[0m"

# Create build dir if not exists
if [ ! -d "build" ]; then
  mkdir build
fi

# Fail if couldn't cd into dir
cd build || exit 1

# Create version dir
if [ ! -d $CONFIGURATION ]; then
  mkdir $CONFIGURATION
fi

# Fail if couldn't cd into dir
cd $CONFIGURATION || exit 1

# Run CMake
cmake -DBUILD_TESTS=$TESTS -DBUILD_DOC=$DOC ../.. $BUILD_SYSTEM -DCMAKE_BUILD_TYPE=$CONFIGURATION -DCMAKE_INSTALL_PREFIX=./bin -DCMAKE_EXPORT_COMPILE_COMMANDS=ON || exit 1

if [ $CHECK_ONLY = 1 ]; then
  exit 0
fi

if [ -z $BUILD_SYSTEM ]; then
  make -j 4 || exit 1
else
  ninja || exit 1
fi

# Install game
if [ $INSTALL = 1 ]; then
  if [ -z $BUILD_SYSTEM ]; then
    make install || exit 1
  else
    ninja install || exit 1
  fi
fi

# Exit script successfully
exit 0
