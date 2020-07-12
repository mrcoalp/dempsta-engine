import sys
import subprocess

args = sys.argv

if sys.platform == "win32":
    args[0] = "utils\\windows\\build.bat"
    subprocess.call(args, cwd=".")
elif sys.platform == "linux" or sys.platform == "linux2":
    args[0] = "utils/linux/build.sh"
    subprocess.call(args, cwd=".")
