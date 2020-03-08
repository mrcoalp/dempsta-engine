#!/bin/bash

# Default is debug build
BUILD=Debug
# Give a different name to your app
APPNAME=game_test

# Check for arguments
while test $# -gt 0; do
  case "$1" in
  -d | --debug | --Debug)
    BUILD=Debug
    ;;
  -r | --release | --Release)
    BUILD=Release
    ;;
  -n | --name)
    shift
    APPNAME=$1
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

# Build project if build folder is not present
if [ ! -d "build/${BUILD}" ]; then
  ./build.sh "--${BUILD}"
fi

# Run the game
./build/${BUILD}/"${APPNAME}"
