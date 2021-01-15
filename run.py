#!/usr/bin/python3

import sys
import subprocess

args = sys.argv

if sys.platform == "win32":
    args[0] = "utils\\windows\\run.bat"
elif sys.platform == "linux" or sys.platform == "linux2":
    args[0] = "utils/linux/run.sh"
else:
    sys.exit()

subprocess.call(args, cwd=".")
