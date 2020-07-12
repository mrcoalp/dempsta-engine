@echo off

set configuration=Debug
set tests=OFF
set clean=false

:loop
    if "%~1" == "" goto end
    if %1 == -d set configuration=Debug
    if %1 == -r set configuration=Release
    if %1 == -t set tests=ON
    if %1 == -c set clean=true
    if %1 == -h call :usage
    shift

if not "%~1" == "" goto loop
:end

if %clean% == true (
    rmdir /s /q build\%configuration%
    echo Removed %cd%\build\%configuration%!
)

mkdir build\%configuration%
cd build\%configuration%

set CC="cl.exe"
set CXX="cl.exe"

cmake -GNinja -DBUILD_TESTS=%tests% -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=%configuration% -DCMAKE_INSTALL_PREFIX=./bin ../..
cmake --build . --target install

cd ../..

exit 0

:usage
    echo "Usage"
    echo "  build [options]"
    echo "  Builds the game, by default, with debug configuration."
    echo "Options"
    echo "  -h  - Show help"
    echo "  -d  - Build game with debug configuration"
    echo "  -r  - Build game with release configuration"
    echo "  -c  - Clean build of configuration"
    echo "  -t  - Build tests"
    exit 0
