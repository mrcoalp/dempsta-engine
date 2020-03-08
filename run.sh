#!/bin/bash

# Default is debug build
BUILD=Debug
# Give a different name to your app
APPNAME=game_test

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
    rm -rf build-*
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

# Run the game
./build/${BUILD}/"${APPNAME}"
