#!/bin/bash

# Default is debug build
CONFIGURATION=Debug
# Give a different name to your app
APPNAME=dempsta_editor
# Build before running?
BUILD=0

usage() {
  echo "Usage

    run.sh [options]

    Runs the game, by default, with debug configuration.
    If build folder doesn't exist, builds game before running.

Options
    -d | --debug        - Run game with debug configuration
    -r | --release      - Run game with release configuration
    -b | --build        - Build configuration before running
    -n | --name <name>  - Specify executable name to run
    -h | --help         - Show help"
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
  -b | --build)
    BUILD=1
    ;;
  -n | --name)
    shift
    APPNAME=$1
    ;;
  -h | --help)
    usage
    exit 0
    ;;
  -*)
    echo -e "\033[0;31mBad argument: $1!\033[0m"
    usage
    exit 1
    ;;
  *)
    echo -e "\033[0;31mUnknown argument: $1!\033[0m"
    usage
    exit 1
    ;;
  esac
  shift
done

# Build project if build folder is not present
if [ ! -d "build/$CONFIGURATION" ] || [ $BUILD = 1 ]; then
  ./$(dirname "$0")/build.sh "--$CONFIGURATION"
fi

if [ -f "build/$CONFIGURATION/$APPNAME" ]; then
  # Run the game
  echo -e "\033[0;34mRunning $CONFIGURATION configuration...\033[0m"
  if [ $CONFIGURATION = Debug ]; then
    gdb build/$CONFIGURATION/"$APPNAME" -ex=run
  else
    ./build/$CONFIGURATION/"$APPNAME"
  fi
  exit 0
else
  echo -e "\033[0;31mExecutable doesn't exist: ./build/$CONFIGURATION/$APPNAME\033[0m"
  exit 1
fi
