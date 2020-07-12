#!/usr/bin/python3

import sys
import subprocess

args = sys.argv

if sys.platform == "win32":
    print("Windows test not yet supported")
elif sys.platform == "linux" or sys.platform == "linux2":
    args[0] = "utils/linux/test.sh"
    subprocess.call(args, cwd=".")
