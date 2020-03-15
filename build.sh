#!/bin/bash

# Default is debug build
CONFIGURATION=Debug
# Install game?
INSTALL=0
# Use Ninja?
NINJA=

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
    NINJA=-GNinja
    ;;
  -h | --help)
    printf "Usage

  build.sh [options]

  Builds the game by default with debug configuration.

  Options
  \t-d | --debug\t\t- Build game with debug configuration
  \t-r | --release\t\t- Build game with release configuration
  \t-c | --clean\t\t- Clean build of configuration
  \t-i | --install\t\t- Install game to build/bin folder
  \t-h | --help\t\t- Show help\n"
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
cmake ../.. -DCMAKE_BUILD_TYPE=$CONFIGURATION -DCMAKE_INSTALL_PREFIX=./bin $NINJA
if [ -z $NINJA ]; then
  make
else
  ninja
fi

# Install game
if [ $INSTALL = 1 ]; then
  if [ -z $NINJA ]; then
    make install
  else
    ninja install
  fi
fi

# Exit script successfully
exit 0
