#!/usr/bin/python3

import sys
import subprocess

args = sys.argv

if sys.platform == "win32":
    print("Windows check not yet supported")
elif sys.platform == "linux" or sys.platform == "linux2":
    args[0] = "utils/linux/check.sh"
else:
    sys.exit()

subprocess.call(args, cwd=".")
