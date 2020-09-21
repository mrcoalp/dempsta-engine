@echo off

set configuration=Debug

:loop
    if "%~1" == "" goto end
    if %1 == -d set configuration=Debug
    if %1 == -r set configuration=Release
    if %1 == -h call :usage
    shift

if not "%~1" == "" goto loop
:end

cd build/%configuration%/
dempsta_editor

exit 0

:usage
    echo "Usage"
    echo "  run [options]"
    echo "  Runs the game, by default, with debug configuration."
    echo "Options"
    echo "  -h  - Show help"
    echo "  -d  - Run game with debug configuration"
    echo "  -r  - Run game with release configuration"
    exit 0
