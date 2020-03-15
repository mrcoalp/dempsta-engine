#!/bin/bash

# Default is debug build
CONFIGURATION=Debug
# Give a different name to your app
APPNAME=game_test
# Build before running?
BUILD=0

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
    printf "Usage

  run.sh [options]

  Runs the game by default with debug configuration.
  If build folder doesn't exist, builds game before running.

  Options
  \t-d | --debug\t\t- Run game with debug configuration
  \t-r | --release\t\t- Run game with release configuration
  \t-b | --build\t\t- Build configuration before running
  \t-n | --name <name>\t- Specify executable name to run
  \t-h | --help\t\t- Show help\n"
    exit 0
    ;;
  -*)
    echo -e "\033[0;31mBad argument: $1!\033[0m"
    ./run.sh -h
    exit 1
    ;;
  *)
    echo -e "\033[0;31mUnknown argument: $1!\033[0m"
    ./run.sh -h
    exit 1
    ;;
  esac
  shift
done

# Build project if build folder is not present
if [ ! -d "build/$CONFIGURATION" ] || [ $BUILD = 1 ]; then
  ./build.sh "--$CONFIGURATION"
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
