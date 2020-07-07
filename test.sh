#!/bin/bash

# Wether or not to output results to file
COVERAGE=0

usage() {
    echo "Usage

    test.sh [options]

    Runs tests defined under test folder.

Options
    -h | --help     - Show help
    -c | --coverage - Output results to file"
}

# Check for arguments
while test $# -gt 0; do
    case "$1" in
    -c | --coverage)
        COVERAGE=1
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

cd build/Debug/test || exit 1
if [ $COVERAGE = 1 ]; then
    ./dempsta_editor_tst | tee '../../../test_coverage.txt' || exit 1
else
    ./dempsta_editor_tst || exit 1
fi
exit 0
