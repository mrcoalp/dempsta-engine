#!/bin/bash

FIX=
FORMAT=0

usage() {
  echo "Usage

    check.sh [options]

    Checks source code with clang-tidy.

Options
    -h | --help     - Show help
    -f | --fix      - Tries to fix warnings or errors found
    --format        - Use clang-format"
}

# Check for arguments
while test $# -gt 0; do
  case "$1" in
  -f | --fix)
    FIX=--fix
    ;;
  --format)
    FORMAT=1
    ;;
  -h | --help)
    usage
    exit 0
    ;;
  -*)
    echo -e "\033[0;31mBad argument: $1\033[0m"
    ./check.sh -h
    exit 1
    ;;
  *)
    echo -e "\033[0;31mUnknown argument: $1\033[0m"
    ./check.sh -h
    exit 1
    ;;
  esac
  shift
done

find engine/src -regex '.*\.\(cpp\)' | xargs clang-format -output-replacements-xml | grep "<replacement " >'format_coverage.txt'
find engine/include -regex '.*\.\(h\)' | xargs clang-format -output-replacements-xml | grep "<replacement " >>'format_coverage.txt'
find editor -regex '.*\.\(cpp\|h\)' | xargs clang-format -output-replacements-xml | grep "<replacement " >>'format_coverage.txt'

# Check if format_coverage.txt exists and is not empty
if [ -s format_coverage.txt ]; then
  echo -e '\033[1;33mStyle corrections must be made. Please use clang-format and commit again!\033[0m'
fi

mv build/Debug/compile_commands.json . || exit 1
find engine/src -regex '.*\.\(cpp\)' | xargs clang-tidy $FIX >'tidy_coverage.txt' || exit 1
find editor -regex '.*\.\(cpp\|h\)' | xargs clang-tidy $FIX >>'tidy_coverage.txt' || exit 1
mv compile_commands.json build/Debug/

# Check if tidy_coverage.txt exists and is not empty
if [ -s tidy_coverage.txt ]; then
  echo -e '\033[1;33mErrors and/or warnings were found. Please check "tidy_coverage.txt"!\033[0m'
fi

if [ $FORMAT = 1 ]; then
  find engine/src -regex '.*\.\(cpp\)' | xargs clang-format -i
  find engine/include -regex '.*\.\(h\)' | xargs clang-format -i
  find editor -regex '.*\.\(cpp\|h\)' | xargs clang-format -i
  echo -e '\033[0;32mRan clang-format!\033[0m'
fi

exit 0
